
"""
Database Management Library

This module provides a comprehensive database management library with support for
multiple database systems, query building, connection pooling, and ORM-like functionality.

Features:
- Multiple database support (SQLite, PostgreSQL, MySQL)
- Connection pooling
- Query builder
- Transaction management
- Migration system
- Schema introspection
- ORM-like model definitions
- Data validation
- Caching layer
- Connection failover
"""

import os
import sys
import json
import time
import sqlite3
import hashlib
import logging
import datetime
import inspect
from typing import List, Dict, Tuple, Optional, Union, Any, Callable, Type
from enum import Enum
from dataclasses import dataclass, field
from abc import ABC, abstractmethod

# Try to import popular database drivers
try:
    import psycopg2
    POSTGRESQL_AVAILABLE = True
except ImportError:
    POSTGRESQL_AVAILABLE = False

try:
    import mysql.connector
    MYSQL_AVAILABLE = True
except ImportError:
    MYSQL_AVAILABLE = False

try:
    import pymongo
    MONGODB_AVAILABLE = True
except ImportError:
    MONGODB_AVAILABLE = False


# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class DatabaseType(Enum):
    """Enumeration of supported database types."""
    SQLITE = "sqlite"
    POSTGRESQL = "postgresql"
    MYSQL = "mysql"
    MONGODB = "mongodb"


class QueryOperator(Enum):
    """Enumeration of query operators."""
    EQUALS = "="
    NOT_EQUALS = "!="
    GREATER_THAN = ">"
    GREATER_THAN_OR_EQUAL = ">="
    LESS_THAN = "<"
    LESS_THAN_OR_EQUAL = "<="
    LIKE = "LIKE"
    IN = "IN"
    NOT_IN = "NOT IN"
    IS_NULL = "IS NULL"
    IS_NOT_NULL = "IS NOT NULL"
    BETWEEN = "BETWEEN"
    AND = "AND"
    OR = "OR"


class JoinType(Enum):
    """Enumeration of join types."""
    INNER = "INNER JOIN"
    LEFT = "LEFT JOIN"
    RIGHT = "RIGHT JOIN"
    FULL = "FULL JOIN"
    CROSS = "CROSS JOIN"


class OrderDirection(Enum):
    """Enumeration of order directions."""
    ASC = "ASC"
    DESC = "DESC"


class TransactionIsolationLevel(Enum):
    """Enumeration of transaction isolation levels."""
    READ_UNCOMMITTED = "READ UNCOMMITTED"
    READ_COMMITTED = "READ COMMITTED"
    REPEATABLE_READ = "REPEATABLE READ"
    SERIALIZABLE = "SERIALIZABLE"


@dataclass
class ColumnInfo:
    """Information about a database column."""
    name: str
    data_type: str
    is_primary_key: bool = False
    is_foreign_key: bool = False
    foreign_key_table: Optional[str] = None
    foreign_key_column: Optional[str] = None
    is_nullable: bool = True
    default_value: Any = None
    max_length: Optional[int] = None
    is_unique: bool = False
    is_indexed: bool = False
    auto_increment: bool = False


@dataclass
class TableInfo:
    """Information about a database table."""
    name: str
    columns: List[ColumnInfo] = field(default_factory=list)
    primary_keys: List[str] = field(default_factory=list)
    foreign_keys: List[Tuple[str, str, str]] = field(default_factory=list)  # (column, ref_table, ref_column)
    indexes: List[List[str]] = field(default_factory=list)


@dataclass
class ConnectionConfig:
    """Database connection configuration."""
    database_type: DatabaseType
    host: Optional[str] = None
    port: Optional[int] = None
    database: Optional[str] = None
    username: Optional[str] = None
    password: Optional[str] = None
    options: Dict[str, Any] = field(default_factory=dict)
    timeout: int = 30
    charset: str = "utf8"
    
    def get_connection_string(self) -> str:
        """
        Generate a connection string from the configuration.
        
        Returns:
            Connection string
        """
        if self.database_type == DatabaseType.SQLITE:
            return self.database or ":memory:"
        
        elif self.database_type == DatabaseType.POSTGRESQL:
            parts = []
            parts.append(f"postgresql://")
            
            if self.username:
                if self.password:
                    parts.append(f"{self.username}:{self.password}@")
                else:
                    parts.append(f"{self.username}@")
            
            host = self.host or "localhost"
            port = self.port or 5432
            database = self.database or "postgres"
            
            parts.append(f"{host}:{port}/{database}")
            
            if self.charset:
                parts.append(f"?charset={self.charset}")
            
            return "".join(parts)
        
        elif self.database_type == DatabaseType.MYSQL:
            parts = []
            parts.append(f"mysql://")
            
            if self.username:
                if self.password:
                    parts.append(f"{self.username}:{self.password}@")
                else:
                    parts.append(f"{self.username}@")
            
            host = self.host or "localhost"
            port = self.port or 3306
            database = self.database or "mysql"
            
            parts.append(f"{host}:{port}/{database}")
            
            if self.charset:
                parts.append(f"?charset={self.charset}")
            
            return "".join(parts)
        
        elif self.database_type == DatabaseType.MONGODB:
            host = self.host or "localhost"
            port = self.port or 27017
            database = self.database or "test"
            
            return f"mongodb://{host}:{port}/{database}"
        
        return ""


class DatabaseConnection(ABC):
    """Abstract base class for database connections."""
    
    def __init__(self, config: ConnectionConfig):
        """
        Initialize the database connection.
        
        Args:
            config: Connection configuration
        """
        self.config = config
        self.is_connected = False
        self.cursor = None
        self.connection = None
        self.in_transaction = False
    
    @abstractmethod
    def connect(self) -> bool:
        """
        Connect to the database.
        
        Returns:
            True if connected successfully, False otherwise
        """
        pass
    
    @abstractmethod
    def disconnect(self) -> bool:
        """
        Disconnect from the database.
        
        Returns:
            True if disconnected successfully, False otherwise
        """
        pass
    
    @abstractmethod
    def execute(self, query: str, params: Optional[Tuple] = None) -> Any:
        """
        Execute a SQL query.
        
        Args:
            query: SQL query to execute
            params: Query parameters
            
        Returns:
            Query result
        """
        pass
    
    @abstractmethod
    def fetch_one(self) -> Optional[Dict]:
        """
        Fetch one row from the result set.
        
        Returns:
            Row as a dictionary or None if no more rows
        """
        pass
    
    @abstractmethod
    def fetch_all(self) -> List[Dict]:
        """
        Fetch all rows from the result set.
        
        Returns:
            List of rows as dictionaries
        """
        pass
    
    @abstractmethod
    def fetch_many(self, size: int) -> List[Dict]:
        """
        Fetch a specific number of rows from the result set.
        
        Args:
            size: Number of rows to fetch
            
        Returns:
            List of rows as dictionaries
        """
        pass
    
    @abstractmethod
    def begin_transaction(self, isolation_level: Optional[TransactionIsolationLevel] = None) -> bool:
        """
        Begin a transaction.
        
        Args:
            isolation_level: Transaction isolation level
            
        Returns:
            True if transaction started successfully, False otherwise
        """
        pass
    
    @abstractmethod
    def commit_transaction(self) -> bool:
        """
        Commit the current transaction.
        
        Returns:
            True if committed successfully, False otherwise
        """
        pass
    
    @abstractmethod
    def rollback_transaction(self) -> bool:
        """
        Roll back the current transaction.
        
        Returns:
            True if rolled back successfully, False otherwise
        """
        pass
    
    @abstractmethod
    def get_last_insert_id(self) -> Optional[int]:
        """
        Get the ID of the last inserted row.
        
        Returns:
            Last insert ID or None
        """
        pass
    
    @abstractmethod
    def get_affected_rows(self) -> int:
        """
        Get the number of affected rows from the last operation.
        
        Returns:
            Number of affected rows
        """
        pass
    
    @abstractmethod
    def escape_identifier(self, identifier: str) -> str:
        """
        Escape a database identifier (table name, column name, etc.).
        
        Args:
            identifier: Identifier to escape
            
        Returns:
            Escaped identifier
        """
        pass
    
    @abstractmethod
    def escape_value(self, value: Any) -> str:
        """
        Escape a value for inclusion in a query.
        
        Args:
            value: Value to escape
            
        Returns:
            Escaped value
        """
        pass
    
    @abstractmethod
    def get_table_info(self, table_name: str) -> Optional[TableInfo]:
        """
        Get information about a table.
        
        Args:
            table_name: Name of the table
            
        Returns:
            Table information or None if table doesn't exist
        """
        pass
    
    @abstractmethod
    def get_table_list(self) -> List[str]:
        """
        Get a list of tables in the database.
        
        Returns:
            List of table names
        """
        pass


class SQLiteConnection(DatabaseConnection):
    """SQLite database connection implementation."""
    
    def __init__(self, config: ConnectionConfig):
        """
        Initialize the SQLite connection.
        
        Args:
            config: Connection configuration
        """
        super().__init__(config)
        self.row_factory = sqlite3.Row
    
    def connect(self) -> bool:
        """
        Connect to the SQLite database.
        
        Returns:
            True if connected successfully, False otherwise
        """
        try:
            db_path = self.config.database or ":memory:"
            
            # Ensure the database directory exists
            if db_path != ":memory:" and os.path.dirname(db_path):
                os.makedirs(os.path.dirname(db_path), exist_ok=True)
            
            self.connection = sqlite3.connect(
                db_path,
                timeout=self.config.timeout,
                check_same_thread=False
            )
            
            # Set row factory to return dictionaries
            self.connection.row_factory = sqlite3.Row
            
            # Enable foreign keys
            self.connection.execute("PRAGMA foreign_keys = ON")
            
            self.is_connected = True
            logger.info(f"Connected to SQLite database: {db_path}")
            
            return True
        except Exception as e:
            logger.error(f"Failed to connect to SQLite database: {str(e)}")
            return False
    
    def disconnect(self) -> bool:
        """
        Disconnect from the SQLite database.
        
        Returns:
            True if disconnected successfully, False otherwise
        """
        try:
            if self.connection:
                self.connection.close()
                self.is_connected = False
                self.connection = None
                self.cursor = None
                
                logger.info("Disconnected from SQLite database")
                return True
        except Exception as e:
            logger.error(f"Failed to disconnect from SQLite database: {str(e)}")
        
        return False
    
    def execute(self, query: str, params: Optional[Tuple] = None) -> Any:
        """
        Execute a SQL query.
        
        Args:
            query: SQL query to execute
            params: Query parameters
            
        Returns:
            Query result
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor()
            
            if params:
                result = cursor.execute(query, params)
            else:
                result = cursor.execute(query)
            
            self.cursor = cursor
            
            # Get the last row ID for INSERT operations
            if query.strip().upper().startswith("INSERT"):
                last_id = cursor.lastrowid
                return cursor, last_id
            
            return cursor
        except Exception as e:
            logger.error(f"Failed to execute query: {query}, params: {params}, error: {str(e)}")
            raise
    
    def fetch_one(self) -> Optional[Dict]:
        """
        Fetch one row from the result set.
        
        Returns:
            Row as a dictionary or None if no more rows
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        row = self.cursor.fetchone()
        if row:
            return dict(row)
        
        return None
    
    def fetch_all(self) -> List[Dict]:
        """
        Fetch all rows from the result set.
        
        Returns:
            List of rows as dictionaries
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        rows = self.cursor.fetchall()
        return [dict(row) for row in rows]
    
    def fetch_many(self, size: int) -> List[Dict]:
        """
        Fetch a specific number of rows from the result set.
        
        Args:
            size: Number of rows to fetch
            
        Returns:
            List of rows as dictionaries
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        rows = self.cursor.fetchmany(size)
        return [dict(row) for row in rows]
    
    def begin_transaction(self, isolation_level: Optional[TransactionIsolationLevel] = None) -> bool:
        """
        Begin a transaction.
        
        Args:
            isolation_level: Transaction isolation level (SQLite only supports DEFERRED, IMMEDIATE, EXCLUSIVE)
            
        Returns:
            True if transaction started successfully, False otherwise
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            if isolation_level:
                # SQLite only supports specific isolation levels
                if isolation_level == TransactionIsolationLevel.READ_UNCOMMITTED:
                    self.connection.execute("BEGIN IMMEDIATE")
                elif isolation_level == TransactionIsolationLevel.READ_COMMITTED:
                    self.connection.execute("BEGIN DEFERRED")
                elif isolation_level == TransactionIsolationLevel.REPEATABLE_READ:
                    self.connection.execute("BEGIN IMMEDIATE")
                elif isolation_level == TransactionIsolationLevel.SERIALIZABLE:
                    self.connection.execute("BEGIN EXCLUSIVE")
                else:
                    self.connection.execute("BEGIN")
            else:
                self.connection.execute("BEGIN")
            
            self.in_transaction = True
            return True
        except Exception as e:
            logger.error(f"Failed to begin transaction: {str(e)}")
            return False
    
    def commit_transaction(self) -> bool:
        """
        Commit the current transaction.
        
        Returns:
            True if committed successfully, False otherwise
        """
        if not self.in_transaction or not self.is_connected or not self.connection:
            raise RuntimeError("No active transaction")
        
        try:
            self.connection.commit()
            self.in_transaction = False
            return True
        except Exception as e:
            logger.error(f"Failed to commit transaction: {str(e)}")
            return False
    
    def rollback_transaction(self) -> bool:
        """
        Roll back the current transaction.
        
        Returns:
            True if rolled back successfully, False otherwise
        """
        if not self.in_transaction or not self.is_connected or not self.connection:
            raise RuntimeError("No active transaction")
        
        try:
            self.connection.rollback()
            self.in_transaction = False
            return True
        except Exception as e:
            logger.error(f"Failed to rollback transaction: {str(e)}")
            return False
    
    def get_last_insert_id(self) -> Optional[int]:
        """
        Get the ID of the last inserted row.
        
        Returns:
            Last insert ID or None
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        return self.connection.lastrowid
    
    def get_affected_rows(self) -> int:
        """
        Get the number of affected rows from the last operation.
        
        Returns:
            Number of affected rows
        """
        if not self.cursor:
            return 0
        
        return self.cursor.rowcount
    
    def escape_identifier(self, identifier: str) -> str:
        """
        Escape a database identifier (table name, column name, etc.).
        
        Args:
            identifier: Identifier to escape
            
        Returns:
            Escaped identifier
        """
        # For SQLite, use double quotes
        return f'"{identifier}"'
    
    def escape_value(self, value: Any) -> str:
        """
        Escape a value for inclusion in a query.
        
        Args:
            value: Value to escape
            
        Returns:
            Escaped value
        """
        if value is None:
            return "NULL"
        elif isinstance(value, bool):
            return "1" if value else "0"
        elif isinstance(value, int) or isinstance(value, float):
            return str(value)
        elif isinstance(value, str):
            return f"'{value.replace("'", "''")}'"
        elif isinstance(value, (list, tuple)):
            escaped_values = [self.escape_value(v) for v in value]
            return f"({', '.join(escaped_values)})"
        else:
            return f"'{str(value)}'"
    
    def get_table_info(self, table_name: str) -> Optional[TableInfo]:
        """
        Get information about a table.
        
        Args:
            table_name: Name of the table
            
        Returns:
            Table information or None if table doesn't exist
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            # Get table schema
            cursor = self.connection.cursor()
            cursor.execute(f"PRAGMA table_info({self.escape_identifier(table_name)})")
            schema_rows = cursor.fetchall()
            
            if not schema_rows:
                return None
            
            # Get foreign key info
            cursor.execute(f"PRAGMA foreign_key_list({self.escape_identifier(table_name)})")
            foreign_key_rows = cursor.fetchall()
            
            # Get index info
            cursor.execute(f"PRAGMA index_list({self.escape_identifier(table_name)})")
            index_list_rows = cursor.fetchall()
            
            # Process columns
            columns = []
            primary_keys = []
            foreign_keys = []
            
            for row in schema_rows:
                col_name = row[1]
                col_type = row[2]
                is_nullable = row[3] == 0  # 0 means nullable
                default_value = row[4]
                is_primary_key = row[5] == 1
                
                column = ColumnInfo(
                    name=col_name,
                    data_type=col_type,
                    is_primary_key=is_primary_key,
                    is_nullable=is_nullable,
                    default_value=default_value
                )
                
                columns.append(column)
                
                if is_primary_key:
                    primary_keys.append(col_name)
            
            # Process foreign keys
            for row in foreign_key_rows:
                from_col = row[3]
                to_table = row[2]
                to_col = row[4]
                
                foreign_keys.append((from_col, to_table, to_col))
                
                # Update column info
                for col in columns:
                    if col.name == from_col:
                        col.is_foreign_key = True
                        col.foreign_key_table = to_table
                        col.foreign_key_column = to_col
                        break
            
            # Process indexes
            indexes = []
            for row in index_list_rows:
                index_name = row[1]
                is_unique = row[2] == 1
                
                cursor.execute(f"PRAGMA index_info({self.escape_identifier(index_name)})")
                index_info_rows = cursor.fetchall()
                
                index_cols = [row[2] for row in index_info_rows]
                indexes.append(index_cols)
            
            return TableInfo(
                name=table_name,
                columns=columns,
                primary_keys=primary_keys,
                foreign_keys=foreign_keys,
                indexes=indexes
            )
        except Exception as e:
            logger.error(f"Failed to get table info for {table_name}: {str(e)}")
            return None
    
    def get_table_list(self) -> List[str]:
        """
        Get a list of tables in the database.
        
        Returns:
            List of table names
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor()
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'")
            tables = cursor.fetchall()
            return [table[0] for table in tables]
        except Exception as e:
            logger.error(f"Failed to get table list: {str(e)}")
            return []


class PostgreSQLConnection(DatabaseConnection):
    """PostgreSQL database connection implementation."""
    
    def __init__(self, config: ConnectionConfig):
        """
        Initialize the PostgreSQL connection.
        
        Args:
            config: Connection configuration
        """
        super().__init__(config)
        if not POSTGRESQL_AVAILABLE:
            raise ImportError("psycopg2 is not available. Install with: pip install psycopg2-binary")
    
    def connect(self) -> bool:
        """
        Connect to the PostgreSQL database.
        
        Returns:
            True if connected successfully, False otherwise
        """
        try:
            connection_string = self.config.get_connection_string()
            
            self.connection = psycopg2.connect(
                connection_string,
                connect_timeout=self.config.timeout
            )
            
            # Set autocommit to False to enable transactions
            self.connection.autocommit = False
            
            self.is_connected = True
            logger.info(f"Connected to PostgreSQL database: {connection_string}")
            
            return True
        except Exception as e:
            logger.error(f"Failed to connect to PostgreSQL database: {str(e)}")
            return False
    
    def disconnect(self) -> bool:
        """
        Disconnect from the PostgreSQL database.
        
        Returns:
            True if disconnected successfully, False otherwise
        """
        try:
            if self.connection:
                self.connection.close()
                self.is_connected = False
                self.connection = None
                self.cursor = None
                
                logger.info("Disconnected from PostgreSQL database")
                return True
        except Exception as e:
            logger.error(f"Failed to disconnect from PostgreSQL database: {str(e)}")
        
        return False
    
    def execute(self, query: str, params: Optional[Tuple] = None) -> Any:
        """
        Execute a SQL query.
        
        Args:
            query: SQL query to execute
            params: Query parameters
            
        Returns:
            Query result
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor()
            
            if params:
                result = cursor.execute(query, params)
            else:
                result = cursor.execute(query)
            
            self.cursor = cursor
            return cursor
        except Exception as e:
            logger.error(f"Failed to execute query: {query}, params: {params}, error: {str(e)}")
            raise
    
    def fetch_one(self) -> Optional[Dict]:
        """
        Fetch one row from the result set.
        
        Returns:
            Row as a dictionary or None if no more rows
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        row = self.cursor.fetchone()
        if row:
            columns = [desc[0] for desc in self.cursor.description]
            return dict(zip(columns, row))
        
        return None
    
    def fetch_all(self) -> List[Dict]:
        """
        Fetch all rows from the result set.
        
        Returns:
            List of rows as dictionaries
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        rows = self.cursor.fetchall()
        columns = [desc[0] for desc in self.cursor.description]
        
        return [dict(zip(columns, row)) for row in rows]
    
    def fetch_many(self, size: int) -> List[Dict]:
        """
        Fetch a specific number of rows from the result set.
        
        Args:
            size: Number of rows to fetch
            
        Returns:
            List of rows as dictionaries
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        rows = self.cursor.fetchmany(size)
        columns = [desc[0] for desc in self.cursor.description]
        
        return [dict(zip(columns, row)) for row in rows]
    
    def begin_transaction(self, isolation_level: Optional[TransactionIsolationLevel] = None) -> bool:
        """
        Begin a transaction.
        
        Args:
            isolation_level: Transaction isolation level
            
        Returns:
            True if transaction started successfully, False otherwise
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            if isolation_level:
                self.connection.set_isolation_level(isolation_level.value)
            else:
                self.connection.set_isolation_level(TransactionIsolationLevel.READ_COMMITTED.value)
            
            self.in_transaction = True
            return True
        except Exception as e:
            logger.error(f"Failed to begin transaction: {str(e)}")
            return False
    
    def commit_transaction(self) -> bool:
        """
        Commit the current transaction.
        
        Returns:
            True if committed successfully, False otherwise
        """
        if not self.in_transaction or not self.is_connected or not self.connection:
            raise RuntimeError("No active transaction")
        
        try:
            self.connection.commit()
            self.in_transaction = False
            return True
        except Exception as e:
            logger.error(f"Failed to commit transaction: {str(e)}")
            return False
    
    def rollback_transaction(self) -> bool:
        """
        Roll back the current transaction.
        
        Returns:
            True if rolled back successfully, False otherwise
        """
        if not self.in_transaction or not self.is_connected or not self.connection:
            raise RuntimeError("No active transaction")
        
        try:
            self.connection.rollback()
            self.in_transaction = False
            return True
        except Exception as e:
            logger.error(f"Failed to rollback transaction: {str(e)}")
            return False
    
    def get_last_insert_id(self) -> Optional[int]:
        """
        Get the ID of the last inserted row.
        
        Returns:
            Last insert ID or None
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor()
            cursor.execute("SELECT lastval()")
            result = cursor.fetchone()
            return result[0] if result else None
        except Exception as e:
            logger.error(f"Failed to get last insert ID: {str(e)}")
            return None
    
    def get_affected_rows(self) -> int:
        """
        Get the number of affected rows from the last operation.
        
        Returns:
            Number of affected rows
        """
        if not self.cursor:
            return 0
        
        return self.cursor.rowcount
    
    def escape_identifier(self, identifier: str) -> str:
        """
        Escape a database identifier (table name, column name, etc.).
        
        Args:
            identifier: Identifier to escape
            
        Returns:
            Escaped identifier
        """
        # For PostgreSQL, use double quotes
        return f'"{identifier}"'
    
    def escape_value(self, value: Any) -> str:
        """
        Escape a value for inclusion in a query.
        
        Args:
            value: Value to escape
            
        Returns:
            Escaped value
        """
        if value is None:
            return "NULL"
        elif isinstance(value, bool):
            return "TRUE" if value else "FALSE"
        elif isinstance(value, int) or isinstance(value, float):
            return str(value)
        elif isinstance(value, str):
            return f"'{value.replace("'", "''")}'"
        elif isinstance(value, (list, tuple)):
            escaped_values = [self.escape_value(v) for v in value]
            return f"({', '.join(escaped_values)})"
        else:
            return f"'{str(value)}'"
    
    def get_table_info(self, table_name: str) -> Optional[TableInfo]:
        """
        Get information about a table.
        
        Args:
            table_name: Name of the table
            
        Returns:
            Table information or None if table doesn't exist
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor()
            
            # Get column info
            cursor.execute("""
                SELECT column_name, data_type, is_nullable, column_default
                FROM information_schema.columns
                WHERE table_schema = CURRENT_SCHEMA() AND table_name = %s
                ORDER BY ordinal_position
            """, (table_name,))
            
            column_rows = cursor.fetchall()
            
            if not column_rows:
                return None
            
            # Get primary key info
            cursor.execute("""
                SELECT column_name
                FROM information_schema.table_constraints tc
                JOIN information_schema.key_column_usage kcu
                ON tc.constraint_name = kcu.constraint_name
                WHERE tc.table_schema = CURRENT_SCHEMA() AND tc.table_name = %s AND tc.constraint_type = 'PRIMARY KEY'
            """, (table_name,))
            
            primary_key_rows = cursor.fetchall()
            primary_keys = [row[0] for row in primary_key_rows]
            
            # Get foreign key info
            cursor.execute("""
                SELECT
                    kcu.column_name,
                    ccu.table_name AS foreign_table_name,
                    ccu.column_name AS foreign_column_name
                FROM information_schema.table_constraints AS tc
                JOIN information_schema.key_column_usage AS kcu
                ON tc.constraint_name = kcu.constraint_name
                JOIN information_schema.constraint_column_usage AS ccu
                ON ccu.constraint_name = tc.constraint_name
                WHERE tc.constraint_type = 'FOREIGN KEY'
                AND tc.table_schema = CURRENT_SCHEMA()
                AND tc.table_name = %s
            """, (table_name,))
            
            foreign_key_rows = cursor.fetchall()
            foreign_keys = [(row[0], row[1], row[2]) for row in foreign_key_rows]
            
            # Get index info
            cursor.execute("""
                SELECT
                    indexname,
                    indexdef
                FROM pg_indexes
                WHERE schemaname = CURRENT_SCHEMA() AND tablename = %s
            """, (table_name,))
            
            index_rows = cursor.fetchall()
            indexes = []
            
            for row in index_rows:
                index_def = row[1]
                # Extract column names from index definition
                if "(" in index_def and ")" in index_def:
                    columns_str = index_def[index_def.find("(") + 1:index_def.rfind(")")]
                    index_columns = [col.strip().replace('"', '') for col in columns_str.split(",")]
                    indexes.append(index_columns)
            
            # Process columns
            columns = []
            for row in column_rows:
                col_name = row[0]
                col_type = row[1]
                is_nullable = row[2] == "YES"
                default_value = row[3]
                is_primary_key = col_name in primary_keys
                
                # Check if it's a foreign key
                is_foreign_key = False
                foreign_key_table = None
                foreign_key_column = None
                
                for fk_col, fk_table, fk_column in foreign_keys:
                    if fk_col == col_name:
                        is_foreign_key = True
                        foreign_key_table = fk_table
                        foreign_key_column = fk_column
                        break
                
                # Check if it's unique
                is_unique = False
                for index_cols in indexes:
                    if col_name in index_cols and len(index_cols) == 1:
                        is_unique = True
                        break
                
                # Check if it's indexed
                is_indexed = any(col_name in index_cols for index_cols in indexes)
                
                column = ColumnInfo(
                    name=col_name,
                    data_type=col_type,
                    is_primary_key=is_primary_key,
                    is_foreign_key=is_foreign_key,
                    foreign_key_table=foreign_key_table,
                    foreign_key_column=foreign_key_column,
                    is_nullable=is_nullable,
                    default_value=default_value,
                    is_unique=is_unique,
                    is_indexed=is_indexed
                )
                
                columns.append(column)
            
            return TableInfo(
                name=table_name,
                columns=columns,
                primary_keys=primary_keys,
                foreign_keys=foreign_keys,
                indexes=indexes
            )
        except Exception as e:
            logger.error(f"Failed to get table info for {table_name}: {str(e)}")
            return None
    
    def get_table_list(self) -> List[str]:
        """
        Get a list of tables in the database.
        
        Returns:
            List of table names
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor()
            cursor.execute("""
                SELECT tablename
                FROM pg_tables
                WHERE schemaname = 'public'
            """)
            
            tables = cursor.fetchall()
            return [table[0] for table in tables]
        except Exception as e:
            logger.error(f"Failed to get table list: {str(e)}")
            return []


class MySQLConnection(DatabaseConnection):
    """MySQL database connection implementation."""
    
    def __init__(self, config: ConnectionConfig):
        """
        Initialize the MySQL connection.
        
        Args:
            config: Connection configuration
        """
        super().__init__(config)
        if not MYSQL_AVAILABLE:
            raise ImportError("mysql.connector is not available. Install with: pip install mysql-connector-python")
    
    def connect(self) -> bool:
        """
        Connect to the MySQL database.
        
        Returns:
            True if connected successfully, False otherwise
        """
        try:
            self.connection = mysql.connector.connect(
                host=self.config.host or "localhost",
                port=self.config.port or 3306,
                database=self.config.database or "mysql",
                user=self.config.username or "root",
                password=self.config.password or "",
                charset=self.config.charset,
                connection_timeout=self.config.timeout,
                autocommit=False
            )
            
            self.is_connected = True
            logger.info(f"Connected to MySQL database: {self.config.database}")
            
            return True
        except Exception as e:
            logger.error(f"Failed to connect to MySQL database: {str(e)}")
            return False
    
    def disconnect(self) -> bool:
        """
        Disconnect from the MySQL database.
        
        Returns:
            True if disconnected successfully, False otherwise
        """
        try:
            if self.connection:
                self.connection.close()
                self.is_connected = False
                self.connection = None
                self.cursor = None
                
                logger.info("Disconnected from MySQL database")
                return True
        except Exception as e:
            logger.error(f"Failed to disconnect from MySQL database: {str(e)}")
        
        return False
    
    def execute(self, query: str, params: Optional[Tuple] = None) -> Any:
        """
        Execute a SQL query.
        
        Args:
            query: SQL query to execute
            params: Query parameters
            
        Returns:
            Query result
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor(dictionary=True)  # Return dictionaries
            
            if params:
                result = cursor.execute(query, params)
            else:
                result = cursor.execute(query)
            
            self.cursor = cursor
            return cursor
        except Exception as e:
            logger.error(f"Failed to execute query: {query}, params: {params}, error: {str(e)}")
            raise
    
    def fetch_one(self) -> Optional[Dict]:
        """
        Fetch one row from the result set.
        
        Returns:
            Row as a dictionary or None if no more rows
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        return self.cursor.fetchone()
    
    def fetch_all(self) -> List[Dict]:
        """
        Fetch all rows from the result set.
        
        Returns:
            List of rows as dictionaries
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        return self.cursor.fetchall()
    
    def fetch_many(self, size: int) -> List[Dict]:
        """
        Fetch a specific number of rows from the result set.
        
        Args:
            size: Number of rows to fetch
            
        Returns:
            List of rows as dictionaries
        """
        if not self.cursor:
            raise RuntimeError("No active query result set")
        
        return self.cursor.fetchmany(size)
    
    def begin_transaction(self, isolation_level: Optional[TransactionIsolationLevel] = None) -> bool:
        """
        Begin a transaction.
        
        Args:
            isolation_level: Transaction isolation level
            
        Returns:
            True if transaction started successfully, False otherwise
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            if isolation_level:
                # Set isolation level
                self.connection.start_transaction(
                    isolation_level=isolation_level.value.replace(" ", "_").lower()
                )
            else:
                self.connection.start_transaction()
            
            self.in_transaction = True
            return True
        except Exception as e:
            logger.error(f"Failed to begin transaction: {str(e)}")
            return False
    
    def commit_transaction(self) -> bool:
        """
        Commit the current transaction.
        
        Returns:
            True if committed successfully, False otherwise
        """
        if not self.in_transaction or not self.is_connected or not self.connection:
            raise RuntimeError("No active transaction")
        
        try:
            self.connection.commit()
            self.in_transaction = False
            return True
        except Exception as e:
            logger.error(f"Failed to commit transaction: {str(e)}")
            return False
    
    def rollback_transaction(self) -> bool:
        """
        Roll back the current transaction.
        
        Returns:
            True if rolled back successfully, False otherwise
        """
        if not self.in_transaction or not self.is_connected or not self.connection:
            raise RuntimeError("No active transaction")
        
        try:
            self.connection.rollback()
            self.in_transaction = False
            return True
        except Exception as e:
            logger.error(f"Failed to rollback transaction: {str(e)}")
            return False
    
    def get_last_insert_id(self) -> Optional[int]:
        """
        Get the ID of the last inserted row.
        
        Returns:
            Last insert ID or None
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            return self.connection.lastrowid
        except Exception as e:
            logger.error(f"Failed to get last insert ID: {str(e)}")
            return None
    
    def get_affected_rows(self) -> int:
        """
        Get the number of affected rows from the last operation.
        
        Returns:
            Number of affected rows
        """
        if not self.cursor:
            return 0
        
        return self.cursor.rowcount
    
    def escape_identifier(self, identifier: str) -> str:
        """
        Escape a database identifier (table name, column name, etc.).
        
        Args:
            identifier: Identifier to escape
            
        Returns:
            Escaped identifier
        """
        # For MySQL, use backticks
        return f"`{identifier}`"
    
    def escape_value(self, value: Any) -> str:
        """
        Escape a value for inclusion in a query.
        
        Args:
            value: Value to escape
            
        Returns:
            Escaped value
        """
        if value is None:
            return "NULL"
        elif isinstance(value, bool):
            return "1" if value else "0"
        elif isinstance(value, int) or isinstance(value, float):
            return str(value)
        elif isinstance(value, str):
            return f"'{value.replace("'", "''")}'"
        elif isinstance(value, (list, tuple)):
            escaped_values = [self.escape_value(v) for v in value]
            return f"({', '.join(escaped_values)})"
        else:
            return f"'{str(value)}'"
    
    def get_table_info(self, table_name: str) -> Optional[TableInfo]:
        """
        Get information about a table.
        
        Args:
            table_name: Name of the table
            
        Returns:
            Table information or None if table doesn't exist
        """
        if not self.is_connected or not self.connection:
            raise RuntimeError("Not connected to database")
        
        try:
            cursor = self.connection.cursor(dictionary=True)
            
            # Get column info
            cursor.execute(f"DESCRIBE {self.escape_identifier(table_name)}")
            column_rows = cursor.fetchall()
            
            if not column_rows:
                return None
            
            # Get primary key info
            cursor.execute(f"""
                SELECT COLUMN_NAME
                FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE
                WHERE TABLE_SCHEMA = %s AND TABLE_NAME = %s
                AND CONSTRAINT_NAME = 'PRIMARY'
            """, (self.config.database or "mysql", table_name))
            
            primary_key_rows = cursor.fetchall()
            primary_keys = [row['COLUMN_NAME'] for row in primary_key_rows]
            
            # Get foreign key info
            cursor.execute(f"""
                SELECT
                    COLUMN_NAME,
                    REFERENCED_TABLE_NAME,
                    REFERENCED_COLUMN_NAME
                FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE
                WHERE TABLE_SCHEMA = %s AND TABLE_NAME = %s
                AND REFERENCED_TABLE_NAME IS NOT NULL
            """, (self.config.database or "mysql", table_name))
            
            foreign_key_rows = cursor.fetchall()
            foreign_keys = [(row['COLUMN_NAME'], row['REFERENCED_TABLE_NAME'], row['REFERENCED_COLUMN_NAME']) for row in foreign_key_rows]
            
            # Get index info
            cursor.execute(f"""
                SHOW INDEX FROM {self.escape_identifier(table_name)}
            """)
            
            index_rows = cursor.fetchall()
            indexes = []
            
            # Group indexes by name
            index_groups = {}
            for row in index_rows:
                index_name = row['Key_name']
                if index_name not in index_groups:
                    index_groups[index_name] = []
                index_groups[index_name].append(row['Column_name'])
            
            # Convert to list of column lists
            for index_name, columns in index_groups.items():
                indexes.append(columns)
            
            # Process columns
            columns = []
            for row in column_rows:
                col_name = row['Field']
                col_type = row['Type']
                is_nullable = row['Null'] == "YES"
                default_value = row['Default']
                is_primary_key = col_name in primary_keys
                
                # Check if it's a foreign key
                is_foreign_key = False
                foreign_key_table = None
                foreign_key_column = None
                
                for fk_col, fk_table, fk_column in foreign_keys:
                    if fk_col == col_name:
                        is_foreign_key = True
                        foreign_key_table = fk_table
                        foreign_key_column = fk_column
                        break
                
                # Check if it's unique
                is_unique = row['Key'] == "UNI"
                
                # Check if it's indexed
                is_indexed = row['Key'] in ("PRI", "UNI", "MUL")
                
                # Check if it's auto increment
                auto_increment = "auto_increment" in str(row.get('Extra', '')).lower()
                
                # Extract max length from type if available
                max_length = None
                if "(" in col_type and ")" in col_type:
                    try:
                        max_length = int(col_type[col_type.find("(") + 1:col_type.rfind(")"])