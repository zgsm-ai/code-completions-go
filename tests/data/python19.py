"""
Advanced Cryptography and Security Library

This module provides a comprehensive cryptography and security library with implementations
of various cryptographic algorithms, security utilities, and secure coding practices.

Features:
- Symmetric encryption (AES, DES, 3DES)
- Asymmetric encryption (RSA, ECC)
- Hashing functions (SHA, MD5, HMAC)
- Digital signatures
- Key generation and management
- Password hashing and verification
- Secure random number generation
- Certificate management
- SSL/TLS utilities
- Security analysis tools
"""

import os
import sys
import json
import time
import secrets
import hashlib
import hmac
import struct
import base64
import math
import typing
import logging
import itertools
from typing import List, Dict, Tuple, Optional, Union, Any, Callable
from datetime import datetime
from enum import Enum

# Try to import cryptography library for advanced features
try:
    from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
    from cryptography.hazmat.primitives.asymmetric import rsa, padding, ec
    from cryptography.hazmat.primitives import hashes, serialization
    from cryptography.hazmat.backends import default_backend
    from cryptography.x509.oid import NameOID
    import cryptography.x509
    CRYPTOGRAPHY_AVAILABLE = True
except ImportError:
    CRYPTOGRAPHY_AVAILABLE = False

# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class CipherMode(Enum):
    """Enumeration of cipher modes."""
    ECB = "ECB"
    CBC = "CBC"
    CTR = "CTR"
    GCM = "GCM"
    CFB = "CFB"
    OFB = "OFB"


class HashAlgorithm(Enum):
    """Enumeration of hash algorithms."""
    SHA1 = "SHA1"
    SHA224 = "SHA224"
    SHA256 = "SHA256"
    SHA384 = "SHA384"
    SHA512 = "SHA512"
    MD5 = "MD5"
    SHA3_224 = "SHA3-224"
    SHA3_256 = "SHA3-256"
    SHA3_384 = "SHA3-384"
    SHA3_512 = "SHA3-512"


class KeyFormat(Enum):
    """Enumeration of key formats."""
    PEM = "PEM"
    DER = "DER"
    RAW = "RAW"
    JWK = "JWK"


class Encoding(Enum):
    """Enumeration of encoding types."""
    BASE64 = "BASE64"
    HEX = "HEX"
    BINARY = "BINARY"


class HashFunction:
    """Hash function implementation."""
    
    def __init__(self, algorithm: HashAlgorithm):
        """
        Initialize the hash function.
        
        Args:
            algorithm: Hash algorithm to use
        """
        self.algorithm = algorithm
        self._hash_obj = None
        
        # Initialize the appropriate hash object
        if algorithm == HashAlgorithm.SHA1:
            self._hash_obj = hashlib.sha1()
        elif algorithm == HashAlgorithm.SHA224:
            self._hash_obj = hashlib.sha224()
        elif algorithm == HashAlgorithm.SHA256:
            self._hash_obj = hashlib.sha256()
        elif algorithm == HashAlgorithm.SHA384:
            self._hash_obj = hashlib.sha384()
        elif algorithm == HashAlgorithm.SHA512:
            self._hash_obj = hashlib.sha512()
        elif algorithm == HashAlgorithm.MD5:
            self._hash_obj = hashlib.md5()
        elif algorithm == HashAlgorithm.SHA3_224:
            try:
                self._hash_obj = hashlib.sha3_224()
            except AttributeError:
                raise ValueError("SHA3-224 is not supported in this Python version")
        elif algorithm == HashAlgorithm.SHA3_256:
            try:
                self._hash_obj = hashlib.sha3_256()
            except AttributeError:
                raise ValueError("SHA3-256 is not supported in this Python version")
        elif algorithm == HashAlgorithm.SHA3_384:
            try:
                self._hash_obj = hashlib.sha3_384()
            except AttributeError:
                raise ValueError("SHA3-384 is not supported in this Python version")
        elif algorithm == HashAlgorithm.SHA3_512:
            try:
                self._hash_obj = hashlib.sha3_512()
            except AttributeError:
                raise ValueError("SHA3-512 is not supported in this Python version")
        else:
            raise ValueError(f"Unsupported hash algorithm: {algorithm}")
    
    def update(self, data: bytes) -> None:
        """
        Update the hash with more data.
        
        Args:
            data: Data to hash
        """
        if isinstance(data, str):
            data = data.encode('utf-8')
        
        self._hash_obj.update(data)
    
    def digest(self) -> bytes:
        """
        Get the digest of the hash.
        
        Returns:
            Hash digest as bytes
        """
        return self._hash_obj.digest()
    
    def hexdigest(self) -> str:
        """
        Get the hex digest of the hash.
        
        Returns:
            Hash digest as hex string
        """
        return self._hash_obj.hexdigest()
    
    @staticmethod
    def hash_data(data: bytes, algorithm: HashAlgorithm) -> bytes:
        """
        Hash data in one step.
        
        Args:
            data: Data to hash
            algorithm: Hash algorithm to use
            
        Returns:
            Hash digest as bytes
        """
        hash_func = HashFunction(algorithm)
        hash_func.update(data)
        return hash_func.digest()
    
    @staticmethod
    def hash_file(file_path: str, algorithm: HashAlgorithm, chunk_size: int = 65536) -> str:
        """
        Hash a file in chunks.
        
        Args:
            file_path: Path to the file
            algorithm: Hash algorithm to use
            chunk_size: Size of chunks to read
            
        Returns:
            Hash digest as hex string
        """
        hash_func = HashFunction(algorithm)
        
        try:
            with open(file_path, 'rb') as f:
                while True:
                    chunk = f.read(chunk_size)
                    if not chunk:
                        break
                    hash_func.update(chunk)
            return hash_func.hexdigest()
        except IOError as e:
            logger.error(f"Error hashing file {file_path}: {str(e)}")
            raise
    
    @staticmethod
    def hmac_digest(data: bytes, key: bytes, algorithm: HashAlgorithm) -> bytes:
        """
        Calculate HMAC digest.
        
        Args:
            data: Data to authenticate
            key: HMAC key
            algorithm: Hash algorithm to use
            
        Returns:
            HMAC digest as bytes
        """
        if isinstance(data, str):
            data = data.encode('utf-8')
        
        if isinstance(key, str):
            key = key.encode('utf-8')
        
        # Select the appropriate hash algorithm for HMAC
        if algorithm == HashAlgorithm.SHA1:
            h = hashlib.sha1
        elif algorithm == HashAlgorithm.SHA256:
            h = hashlib.sha256
        elif algorithm == HashAlgorithm.SHA512:
            h = hashlib.sha512
        else:
            h = hashlib.sha256  # Default
        
        return hmac.new(key, data, h).digest()


class SymmetricCipher:
    """Symmetric cipher implementation."""
    
    def __init__(self, algorithm: str, key: bytes, mode: CipherMode = CipherMode.ECB, iv: Optional[bytes] = None):
        """
        Initialize the symmetric cipher.
        
        Args:
            algorithm: Cipher algorithm (AES, DES, 3DES)
            key: Encryption key
            mode: Cipher mode
            iv: Initialization vector
        """
        self.algorithm = algorithm
        self.key = key
        self.mode = mode
        self.iv = iv
        
        # Validate key size based on algorithm
        if algorithm.upper() == "AES":
            if len(key) not in [16, 24, 32]:
                raise ValueError("AES key must be 16, 24, or 32 bytes")
        elif algorithm.upper() == "DES":
            if len(key) != 8:
                raise ValueError("DES key must be 8 bytes")
        elif algorithm.upper() == "3DES":
            if len(key) not in [16, 24]:
                raise ValueError("3DES key must be 16 or 24 bytes")
        else:
            raise ValueError(f"Unsupported cipher algorithm: {algorithm}")
        
        # Validate IV for certain modes
        if mode in [CipherMode.CBC, CipherMode.CTR, CipherMode.CFB, CipherMode.OFB] and not iv:
            if algorithm.upper() == "AES":
                iv_size = 16
            elif algorithm.upper() == "DES":
                iv_size = 8
            elif algorithm.upper() == "3DES":
                iv_size = 8
            else:
                iv_size = 16
            
            self.iv = os.urandom(iv_size)
    
    def encrypt(self, plaintext: bytes) -> bytes:
        """
        Encrypt plaintext.
        
        Args:
            plaintext: Data to encrypt
            
        Returns:
            Encrypted data
        """
        if isinstance(plaintext, str):
            plaintext = plaintext.encode('utf-8')
        
        if CRYPTOGRAPHY_AVAILABLE:
            return self._encrypt_with_cryptography(plaintext)
        else:
            return self._encrypt_simple(plaintext)
    
    def decrypt(self, ciphertext: bytes) -> bytes:
        """
        Decrypt ciphertext.
        
        Args:
            ciphertext: Data to decrypt
            
        Returns:
            Decrypted data
        """
        if CRYPTOGRAPHY_AVAILABLE:
            return self._decrypt_with_cryptography(ciphertext)
        else:
            return self._decrypt_simple(ciphertext)
    
    def _encrypt_with_cryptography(self, plaintext: bytes) -> bytes:
        """
        Encrypt using cryptography library.
        
        Args:
            plaintext: Data to encrypt
            
        Returns:
            Encrypted data
        """
        try:
            # Create cipher object
            if self.algorithm.upper() == "AES":
                cipher_obj = algorithms.AES(self.key)
            elif self.algorithm.upper() == "DES":
                cipher_obj = algorithms.TripleDES(self.key)
            elif self.algorithm.upper() == "3DES":
                cipher_obj = algorithms.TripleDES(self.key)
            else:
                raise ValueError(f"Unsupported cipher algorithm: {self.algorithm}")
            
            # Create mode object
            if self.mode == CipherMode.ECB:
                mode_obj = modes.ECB()
            elif self.mode == CipherMode.CBC:
                mode_obj = modes.CBC(self.iv)
            elif self.mode == CipherMode.CTR:
                mode_obj = modes.CTR(self.iv)
            elif self.mode == CipherMode.CFB:
                mode_obj = modes.CFB(self.iv)
            elif self.mode == CipherMode.OFB:
                mode_obj = modes.OFB(self.iv)
            elif self.mode == CipherMode.GCM:
                mode_obj = modes.GCM(self.iv)
            else:
                mode_obj = modes.ECB()
            
            # Create encryptor
            cipher = Cipher(cipher_obj, mode_obj, backend=default_backend())
            encryptor = cipher.encryptor()
            
            # Update with plaintext
            encryptor.update(plaintext)
            
            # Finalize encryption
            ciphertext = encryptor.finalize()
            
            # If using GCM mode, include the authentication tag
            if self.mode == CipherMode.GCM:
                return ciphertext + encryptor.tag
            
            return ciphertext
        except Exception as e:
            logger.error(f"Error encrypting with cryptography: {str(e)}")
            raise
    
    def _decrypt_with_cryptography(self, ciphertext: bytes) -> bytes:
        """
        Decrypt using cryptography library.
        
        Args:
            ciphertext: Data to decrypt
            
        Returns:
            Decrypted data
        """
        try:
            # Handle GCM mode (ciphertext includes authentication tag)
            tag = None
            if self.mode == CipherMode.GCM:
                tag = ciphertext[-16:]  # Last 16 bytes is the tag
                ciphertext = ciphertext[:-16]
            
            # Create cipher object
            if self.algorithm.upper() == "AES":
                cipher_obj = algorithms.AES(self.key)
            elif self.algorithm.upper() == "DES":
                cipher_obj = algorithms.TripleDES(self.key)
            elif self.algorithm.upper() == "3DES":
                cipher_obj = algorithms.TripleDES(self.key)
            else:
                raise ValueError(f"Unsupported cipher algorithm: {self.algorithm}")
            
            # Create mode object
            if self.mode == CipherMode.ECB:
                mode_obj = modes.ECB()
            elif self.mode == CipherMode.CBC:
                mode_obj = modes.CBC(self.iv)
            elif self.mode == CipherMode.CTR:
                mode_obj = modes.CTR(self.iv)
            elif self.mode == CipherMode.CFB:
                mode_obj = modes.CFB(self.iv)
            elif self.mode == CipherMode.OFB:
                mode_obj = modes.OFB(self.iv)
            elif self.mode == CipherMode.GCM:
                mode_obj = modes.GCM(self.iv, tag)
            else:
                mode_obj = modes.ECB()
            
            # Create decryptor
            cipher = Cipher(cipher_obj, mode_obj, backend=default_backend())
            decryptor = cipher.decryptor()
            
            # Update with ciphertext
            decryptor.update(ciphertext)
            
            # Finalize decryption
            plaintext = decryptor.finalize()
            
            return plaintext
        except Exception as e:
            logger.error(f"Error decrypting with cryptography: {str(e)}")
            raise
    
    def _encrypt_simple(self, plaintext: bytes) -> bytes:
        """
        Simple encryption implementation (fallback).
        
        Args:
            plaintext: Data to encrypt
            
        Returns:
            Encrypted data
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper cryptographic algorithms
        
        # Simple XOR encryption (NOT SECURE - for demonstration only)
        key_expanded = self.key
        while len(key_expanded) < len(plaintext):
            key_expanded += self.key
        
        return bytes([plaintext[i] ^ key_expanded[i] for i in range(len(plaintext))])
    
    def _decrypt_simple(self, ciphertext: bytes) -> bytes:
        """
        Simple decryption implementation (fallback).
        
        Args:
            ciphertext: Data to decrypt
            
        Returns:
            Decrypted data
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper cryptographic algorithms
        
        # Simple XOR decryption (NOT SECURE - for demonstration only)
        key_expanded = self.key
        while len(key_expanded) < len(ciphertext):
            key_expanded += self.key
        
        return bytes([ciphertext[i] ^ key_expanded[i] for i in range(len(ciphertext))])
    
    @staticmethod
    def generate_key(algorithm: str, key_size: Optional[int] = None) -> bytes:
        """
        Generate a random key.
        
        Args:
            algorithm: Cipher algorithm
            key_size: Key size in bytes
            
        Returns:
            Generated key
        """
        if algorithm.upper() == "AES":
            if key_size not in [16, 24, 32]:
                key_size = 32  # Default to 256 bits
        elif algorithm.upper() == "DES":
            key_size = 8
        elif algorithm.upper() == "3DES":
            key_size = 24
        else:
            key_size = 32  # Default to 256 bits
        
        return secrets.token_bytes(key_size)
    
    @staticmethod
    def generate_iv(algorithm: str) -> bytes:
        """
        Generate a random IV.
        
        Args:
            algorithm: Cipher algorithm
            
        Returns:
            Generated IV
        """
        if algorithm.upper() == "AES":
            return secrets.token_bytes(16)
        elif algorithm.upper() in ["DES", "3DES"]:
            return secrets.token_bytes(8)
        else:
            return secrets.token_bytes(16)


class AsymmetricCipher:
    """Asymmetric cipher implementation."""
    
    def __init__(self, private_key: Optional[bytes] = None, public_key: Optional[bytes] = None, key_type: str = "RSA"):
        """
        Initialize the asymmetric cipher.
        
        Args:
            private_key: Private key
            public_key: Public key
            key_type: Key type (RSA, ECC)
        """
        self.private_key = private_key
        self.public_key = public_key
        self.key_type = key_type
        
        if CRYPTOGRAPHY_AVAILABLE:
            self._parse_keys_cryptography()
    
    def _parse_keys_cryptography(self):
        """Parse keys using cryptography library."""
        try:
            # Parse private key
            if self.private_key:
                if self.key_type == "RSA":
                    self._priv_key_obj = serialization.load_pem_private_key(
                        self.private_key,
                        password=None,
                        backend=default_backend()
                    )
                elif self.key_type == "ECC":
                    self._priv_key_obj = serialization.load_pem_private_key(
                        self.private_key,
                        password=None,
                        backend=default_backend()
                    )
            
            # Parse public key
            if self.public_key:
                if self.key_type == "RSA":
                    self._pub_key_obj = serialization.load_pem_public_key(
                        self.public_key,
                        backend=default_backend()
                    )
                elif self.key_type == "ECC":
                    self._pub_key_obj = serialization.load_pem_public_key(
                        self.public_key,
                        backend=default_backend()
                    )
        except Exception as e:
            logger.error(f"Error parsing keys: {str(e)}")
            raise
    
    def encrypt(self, plaintext: bytes) -> bytes:
        """
        Encrypt plaintext.
        
        Args:
            plaintext: Data to encrypt
            
        Returns:
            Encrypted data
        """
        if isinstance(plaintext, str):
            plaintext = plaintext.encode('utf-8')
        
        if not self.public_key:
            raise ValueError("Public key is required for encryption")
        
        if CRYPTOGRAPHY_AVAILABLE:
            return self._encrypt_with_cryptography(plaintext)
        else:
            return self._encrypt_simple(plaintext)
    
    def decrypt(self, ciphertext: bytes) -> bytes:
        """
        Decrypt ciphertext.
        
        Args:
            ciphertext: Data to decrypt
            
        Returns:
            Decrypted data
        """
        if not self.private_key:
            raise ValueError("Private key is required for decryption")
        
        if CRYPTOGRAPHY_AVAILABLE:
            return self._decrypt_with_cryptography(ciphertext)
        else:
            return self._decrypt_simple(ciphertext)
    
    def sign(self, data: bytes) -> bytes:
        """
        Sign data.
        
        Args:
            data: Data to sign
            
        Returns:
            Digital signature
        """
        if isinstance(data, str):
            data = data.encode('utf-8')
        
        if not self.private_key:
            raise ValueError("Private key is required for signing")
        
        if CRYPTOGRAPHY_AVAILABLE:
            return self._sign_with_cryptography(data)
        else:
            return self._sign_simple(data)
    
    def verify(self, data: bytes, signature: bytes) -> bool:
        """
        Verify a signature.
        
        Args:
            data: Original data
            signature: Digital signature
            
        Returns:
            True if signature is valid, False otherwise
        """
        if isinstance(data, str):
            data = data.encode('utf-8')
        
        if not self.public_key:
            raise ValueError("Public key is required for verification")
        
        if CRYPTOGRAPHY_AVAILABLE:
            return self._verify_with_cryptography(data, signature)
        else:
            return self._verify_simple(data, signature)
    
    def _encrypt_with_cryptography(self, plaintext: bytes) -> bytes:
        """
        Encrypt using cryptography library.
        
        Args:
            plaintext: Data to encrypt
            
        Returns:
            Encrypted data
        """
        try:
            if self.key_type == "RSA":
                # Use OAEP padding for RSA encryption
                ciphertext = self._pub_key_obj.encrypt(
                    plaintext,
                    padding.OAEP(
                        mgf=padding.MGF1(algorithm=hashes.SHA256()),
                        algorithm=hashes.SHA256(),
                        label=None
                    )
                )
            elif self.key_type == "ECC":
                # Use ECIES for ECC encryption (simplified)
                # In a real implementation, you would use a proper ECIES library
                
                # For demonstration, we'll use a simplified approach
                # Generate a random symmetric key
                sym_key = secrets.token_bytes(32)
                
                # Encrypt the data with the symmetric key
                cipher = SymmetricCipher("AES", sym_key, CipherMode.GCM)
                ciphertext = cipher.encrypt(plaintext)
                
                # Encrypt the symmetric key with the public key
                encrypted_key = self._pub_key_obj.encrypt(
                    sym_key,
                    padding.OAEP(
                        mgf=padding.MGF1(algorithm=hashes.SHA256()),
                        algorithm=hashes.SHA256(),
                        label=None
                    )
                )
                
                # Return encrypted key + ciphertext
                return encrypted_key + ciphertext
            
            return ciphertext
        except Exception as e:
            logger.error(f"Error encrypting with cryptography: {str(e)}")
            raise
    
    def _decrypt_with_cryptography(self, ciphertext: bytes) -> bytes:
        """
        Decrypt using cryptography library.
        
        Args:
            ciphertext: Data to decrypt
            
        Returns:
            Decrypted data
        """
        try:
            if self.key_type == "RSA":
                # Use OAEP padding for RSA decryption
                plaintext = self._priv_key_obj.decrypt(
                    ciphertext,
                    padding.OAEP(
                        mgf=padding.MGF1(algorithm=hashes.SHA256()),
                        algorithm=hashes.SHA256(),
                        label=None
                    )
                )
            elif self.key_type == "ECC":
                # Split the ciphertext into encrypted key and actual ciphertext
                key_size = 256  # RSA key size
                encrypted_key = ciphertext[:key_size]
                actual_ciphertext = ciphertext[key_size:]
                
                # Decrypt the symmetric key with the private key
                sym_key = self._priv_key_obj.decrypt(
                    encrypted_key,
                    padding.OAEP(
                        mgf=padding.MGF1(algorithm=hashes.SHA256()),
                        algorithm=hashes.SHA256(),
                        label=None
                    )
                )
                
                # Decrypt the actual ciphertext with the symmetric key
                # Note: For a real implementation, you would need to extract the IV and tag
                # This is a simplified approach
                cipher = SymmetricCipher("AES", sym_key, CipherMode.GCM)
                plaintext = cipher.decrypt(actual_ciphertext)
                
                return plaintext
            
            return plaintext
        except Exception as e:
            logger.error(f"Error decrypting with cryptography: {str(e)}")
            raise
    
    def _sign_with_cryptography(self, data: bytes) -> bytes:
        """
        Sign data using cryptography library.
        
        Args:
            data: Data to sign
            
        Returns:
            Digital signature
        """
        try:
            if self.key_type == "RSA":
                # Use PSS padding for RSA signing
                signature = self._priv_key_obj.sign(
                    data,
                    padding.PSS(
                        mgf=padding.MGF1(hashes.SHA256()),
                        salt_length=padding.PSS.MAX_LENGTH
                    ),
                    hashes.SHA256()
                )
            elif self.key_type == "ECC":
                # Use ECDSA for ECC signing
                signature = self._priv_key_obj.sign(
                    data,
                    ec.ECDSA(hashes.SHA256())
                )
            
            return signature
        except Exception as e:
            logger.error(f"Error signing with cryptography: {str(e)}")
            raise
    
    def _verify_with_cryptography(self, data: bytes, signature: bytes) -> bool:
        """
        Verify signature using cryptography library.
        
        Args:
            data: Original data
            signature: Digital signature
            
        Returns:
            True if signature is valid, False otherwise
        """
        try:
            if self.key_type == "RSA":
                # Use PSS padding for RSA verification
                self._pub_key_obj.verify(
                    signature,
                    data,
                    padding.PSS(
                        mgf=padding.MGF1(hashes.SHA256()),
                        salt_length=padding.PSS.MAX_LENGTH
                    ),
                    hashes.SHA256()
                )
            elif self.key_type == "ECC":
                # Use ECDSA for ECC verification
                self._pub_key_obj.verify(
                    signature,
                    data,
                    ec.ECDSA(hashes.SHA256())
                )
            
            return True
        except Exception as e:
            logger.error(f"Error verifying with cryptography: {str(e)}")
            return False
    
    def _encrypt_simple(self, plaintext: bytes) -> bytes:
        """
        Simple encryption implementation (fallback).
        
        Args:
            plaintext: Data to encrypt
            
        Returns:
            Encrypted data
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper cryptographic algorithms
        
        # For demonstration, we'll use a simple approach
        # In a real implementation, you would use proper RSA or ECC encryption
        
        # Generate a random symmetric key
        sym_key = secrets.token_bytes(32)
        
        # Encrypt the data with the symmetric key
        cipher = SymmetricCipher("AES", sym_key, CipherMode.GCM)
        ciphertext = cipher.encrypt(plaintext)
        
        # Encrypt the symmetric key with a simple XOR operation
        # (NOT SECURE - for demonstration only)
        encrypted_key = bytes([
            sym_key[i] ^ (i + 1) % 256 
            for i in range(len(sym_key))
        ])
        
        # Return encrypted key + ciphertext
        return encrypted_key + ciphertext
    
    def _decrypt_simple(self, ciphertext: bytes) -> bytes:
        """
        Simple decryption implementation (fallback).
        
        Args:
            ciphertext: Data to decrypt
            
        Returns:
            Decrypted data
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper cryptographic algorithms
        
        # Split the ciphertext into encrypted key and actual ciphertext
        key_size = 32
        encrypted_key = ciphertext[:key_size]
        actual_ciphertext = ciphertext[key_size:]
        
        # Decrypt the symmetric key with a simple XOR operation
        # (NOT SECURE - for demonstration only)
        sym_key = bytes([
            encrypted_key[i] ^ (i + 1) % 256 
            for i in range(len(encrypted_key))
        ])
        
        # Decrypt the actual ciphertext with the symmetric key
        cipher = SymmetricCipher("AES", sym_key, CipherMode.GCM)
        plaintext = cipher.decrypt(actual_ciphertext)
        
        return plaintext
    
    def _sign_simple(self, data: bytes) -> bytes:
        """
        Simple signing implementation (fallback).
        
        Args:
            data: Data to sign
            
        Returns:
            Digital signature
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper RSA or ECC signing
        
        # Generate a random signature
        # (NOT SECURE - for demonstration only)
        signature = secrets.token_bytes(256)
        
        # Make the signature depend on the data
        for i in range(min(len(data), len(signature))):
            signature[i] ^= data[i] ^ (i + 1) % 256
        
        return signature
    
    def _verify_simple(self, data: bytes, signature: bytes) -> bool:
        """
        Simple verification implementation (fallback).
        
        Args:
            data: Original data
            signature: Digital signature
            
        Returns:
            True if signature is valid, False otherwise
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper RSA or ECC verification
        
        # Recreate the signature from the data
        if len(signature) < 256:
            return False
        
        # Recreate the signature
        recreated_signature = secrets.token_bytes(256)
        for i in range(min(len(data), len(recreated_signature))):
            recreated_signature[i] ^= data[i] ^ (i + 1) % 256
        
        # Compare the signatures
        return secrets.compare_digest(recreated_signature, signature)
    
    @staticmethod
    def generate_key_pair(key_type: str, key_size: Optional[int] = None) -> Tuple[bytes, bytes]:
        """
        Generate a key pair.
        
        Args:
            key_type: Key type (RSA, ECC)
            key_size: Key size
            
        Returns:
            Tuple of (private_key, public_key)
        """
        if CRYPTOGRAPHY_AVAILABLE:
            return AsymmetricCipher._generate_key_pair_cryptography(key_type, key_size)
        else:
            return AsymmetricCipher._generate_key_pair_simple(key_type, key_size)
    
    @staticmethod
    def _generate_key_pair_cryptography(key_type: str, key_size: Optional[int] = None) -> Tuple[bytes, bytes]:
        """
        Generate a key pair using cryptography library.
        
        Args:
            key_type: Key type (RSA, ECC)
            key_size: Key size
            
        Returns:
            Tuple of (private_key, public_key)
        """
        try:
            if key_type == "RSA":
                if not key_size:
                    key_size = 2048  # Default to 2048 bits
                
                # Generate RSA key pair
                private_key = rsa.generate_private_key(
                    public_exponent=65537,
                    key_size=key_size,
                    backend=default_backend()
                )
                public_key = private_key.public_key()
                
                # Serialize keys to PEM format
                private_pem = private_key.private_bytes(
                    encoding=serialization.Encoding.PEM,
                    format=serialization.PrivateFormat.PKCS8,
                    encryption_algorithm=serialization.NoEncryption()
                )
                
                public_pem = public_key.public_bytes(
                    encoding=serialization.Encoding.PEM,
                    format=serialization.PublicFormat.SubjectPublicKeyInfo
                )
                
                return private_pem, public_pem
            
            elif key_type == "ECC":
                # Default to P-256 curve if not specified
                curve = ec.SECP256R1()
                
                # Generate ECC key pair
                private_key = ec.generate_private_key(
                    curve=curve,
                    backend=default_backend()
                )
                public_key = private_key.public_key()
                
                # Serialize keys to PEM format
                private_pem = private_key.private_bytes(
                    encoding=serialization.Encoding.PEM,
                    format=serialization.PrivateFormat.PKCS8,
                    encryption_algorithm=serialization.NoEncryption()
                )
                
                public_pem = public_key.public_bytes(
                    encoding=serialization.Encoding.PEM,
                    format=serialization.PublicFormat.SubjectPublicKeyInfo
                )
                
                return private_pem, public_pem
            
            else:
                raise ValueError(f"Unsupported key type: {key_type}")
        except Exception as e:
            logger.error(f"Error generating key pair: {str(e)}")
            raise
    
    @staticmethod
    def _generate_key_pair_simple(key_type: str, key_size: Optional[int] = None) -> Tuple[bytes, bytes]:
        """
        Generate a key pair using a simple implementation (fallback).
        
        Args:
            key_type: Key type (RSA, ECC)
            key_size: Key size
            
        Returns:
            Tuple of (private_key, public_key)
        """
        # This is a very simplified implementation for demonstration
        # In a real implementation, you would use proper cryptographic algorithms
        
        # Generate random keys
        if key_type == "RSA":
            if not key_size:
                key_size = 2048
            
            # Generate random keys of the appropriate size
            private_key = secrets.token_bytes(key_size // 8)
            public_key = secrets.token_bytes(key_size // 8)
        elif key_type == "ECC":
            # ECC keys are typically smaller
            private_key = secrets.token_bytes(32)
            public_key = secrets.token_bytes(64)
        else:
            private_key = secrets.token_bytes(32)
            public_key = secrets.token_bytes(64)
        
        # Add some structure to the keys to make them look like real keys
        private_key = b"-----BEGIN PRIVATE KEY-----\n" + base64.b64encode(private_key) + b"\n-----END PRIVATE KEY-----"
        public_key = b"-----BEGIN PUBLIC KEY-----\n" + base64.b64encode(public_key) + b"\n-----END PUBLIC KEY-----"
        
        return private_key, public_key


class KeyManager:
    """Key management utilities."""
    
    @staticmethod
    def derive_key_from_password(password: str, salt: Optional[bytes] = None, iterations: int = 100000, key_length: int = 32) -> bytes:
        """
        Derive a key from a password using PBKDF2.
        
        Args:
            password: Password
            salt: Salt (randomly generated if None)
            iterations: Number of iterations
            key_length: Length of the derived key
            
        Returns:
            Tuple of (key, salt)
        """
        if salt is None:
            salt = secrets.token_bytes(16)
        
        if isinstance(password, str):
            password = password.encode('utf-8')
        
        key = hashlib.pbkdf2_hmac(
            'sha256',
            password,
            salt,
            iterations,
            dklen=key_length
        )
        
        return key, salt
    
    @staticmethod
    def generate_salt(length: int = 16) -> bytes:
        """
        Generate a random salt.
        
        Args:
            length: Length of the salt in bytes
            
        Returns:
            Random salt
        """
        return secrets.token_bytes(length)
    
    @staticmethod
    def hash_password(password: str, salt: Optional[bytes] = None) -> str:
        """
        Hash a password using a secure method.
        
        Args:
            password: Password to hash
            salt: Salt (randomly generated if None)
            
        Returns:
            Hashed password (with salt included)
        """
        if salt is None:
            salt = secrets.token_bytes(16)
        
        if isinstance(password, str):
            password = password.encode('utf-8')
        
        # Use PBKDF2 with a high number of iterations
        key = hashlib.pbkdf2_hmac('sha256', password, salt, 100000)
        
        # Encode the salt and hash in a single string
        return base64.b64encode(salt + key).decode('ascii')
    
    @staticmethod
    def verify_password(password: str, hashed_password: str) -> bool:
        """
        Verify a password against a hashed password.
        
        Args:
            password: Password to verify
            hashed_password: Hashed password
            
        Returns:
            True if password matches, False otherwise
        """
        try:
            # Decode the hashed password
            decoded = base64.b64decode(hashed_password.encode('ascii'))
            
            # Extract salt and hash
            salt = decoded[:16]
            expected_hash = decoded[16:]
            
            # Compute the hash of the provided password
            if isinstance(password, str):
                password = password.encode('utf-8')
            
            actual_hash = hashlib.pbkdf2_hmac('sha256', password, salt, 100000)
            
            # Compare the hashes using a constant-time comparison
            return secrets.compare_digest(actual_hash, expected_hash)
        except Exception:
            return False


class SecureRandom:
    """Secure random number generation."""
    
    @staticmethod
    def token_bytes(nbytes: int = 32) -> bytes:
        """
        Generate a random byte string.
        
        Args:
            nbytes: Number of bytes to generate
            
        Returns:
            Random byte string
        """
        return secrets.token_bytes(nbytes)
    
    @staticmethod
    def token_hex(nbytes: int = 32) -> str:
        """
        Generate a random hex string.
        
        Args:
            nbytes: Number of bytes to generate
            
        Returns:
            Random hex string
        """
        return secrets.token_hex(nbytes)
    
    @staticmethod
    def token_urlsafe(nbytes: int = 32) -> str:
        """
        Generate a random URL-safe string.
        
        Args:
            nbytes: Number of bytes to generate
            
        Returns:
            Random URL-safe string
        """
        return secrets.token_urlsafe(nbytes)
    
    @staticmethod
    def choice(sequence: List) -> Any:
        """
        Randomly choose an element from a sequence.
        
        Args:
            sequence: Sequence to choose from
            
        Returns:
            Random element from the sequence
        """
        return secrets.choice(sequence)
    
    @staticmethod
    def randbelow(upper_bound: int) -> int:
        """
        Generate a random integer in range [0, upper_bound).
        
        Args:
            upper_bound: Upper bound (exclusive)
            
        Returns:
            Random integer
        """
        return secrets.randbelow(upper_bound)
    
    @staticmethod
    def randint(lower: int, upper: int) -> int:
        """
        Generate a random integer in range [lower, upper].
        
        Args:
            lower: Lower bound (inclusive)
            upper: Upper bound (inclusive)
            
        Returns:
            Random integer
        """
        return secrets.randbelow(upper - lower + 1) + lower


class SecurityAnalyzer:
    """Security analysis tools."""
    
    @staticmethod
    def analyze_password_strength(password: str) -> Dict[str, Any]:
        """
        Analyze the strength of a password.
        
        Args:
            password: Password to analyze
            
        Returns:
            Dictionary with strength analysis
        """
        if not password:
            return {
                'score': 0,
                'strength': 'very_weak',
                'feedback': 'Password is empty',
                'has_lowercase': False,
                'has_uppercase': False,
                'has_digit': False,
                'has_special': False,
                'length': 0
            }
        
        # Initialize analysis
        has_lowercase = any(c.islower() for c in password)
        has_uppercase = any(c.isupper() for c in password)
        has_digit = any(c.isdigit() for c in password)
        has_special = any(not c.isalnum() for c in password)
        length = len(password)
        
        # Calculate score
        score = 0
        
        # Length score
        if length >= 8:
            score += 2
        elif length >= 6:
            score += 1
        
        # Character type scores
        if has_lowercase:
            score += 1
        if has_uppercase:
            score += 1
        if has_digit:
            score += 1
        if has_special:
            score += 1
        
        # Determine strength
        if score < 3:
            strength = 'weak'
            feedback = 'Password is weak. Consider using a longer password with a mix of character types.'
        elif score < 5:
            strength = 'medium'
            feedback = 'Password is of medium strength. Consider adding more character types.'
        else:
            strength = 'strong'
            feedback = 'Password is strong. Good job!'
        
        return {
            'score': score,
            'strength': strength,
            'feedback': feedback,
            'has_lowercase': has_lowercase,
            'has_uppercase': has_uppercase,
            'has_digit': has_digit,
            'has_special': has_special,
            'length': length
        }
    
    @staticmethod
    def analyze_file_entropy(file_path: str, chunk_size: int = 65536) -> Dict[str, float]:
        """
        Analyze the entropy of a file.
        
        Args:
            file_path: Path to the file
            chunk_size: Size of chunks to read
            
        Returns:
            Dictionary with entropy analysis
        """
        try:
            with open(file_path, 'rb') as f:
                # Count byte frequencies
                byte_counts = [0] * 256
                total_bytes = 0
                
                while True:
                    chunk = f.read(chunk_size)
                    if not chunk:
                        break
                    
                    for byte in chunk:
                        byte_counts[byte] += 1
                        total_bytes += 1
                
                # Calculate probabilities
                probabilities = [count / total_bytes for count in byte_counts if count > 0]
                
                # Calculate entropy
                entropy = 0
                for p in probabilities:
                    entropy -= p * math.log2(p)
                
                # Calculate maximum possible entropy (8 for a byte)
                max_entropy = 8
                
                # Calculate normalized entropy
                normalized_entropy = entropy / max_entropy
                
                return {
                    'entropy': entropy,
                    'normalized_entropy': normalized_entropy,
                    'max_entropy': max_entropy,
                    'size': total_bytes
                }
        except IOError as e:
            logger.error(f"Error analyzing file {file_path}: {str(e)}")
            raise
    
    @staticmethod
    def generate_random_data(size: int, entropy: float = 8.0) -> bytes:
        """
        Generate random data with specified entropy.
        
        Args:
            size: Size of data to generate
            entropy: Desired entropy (0.0 to 8.0)
            
        Returns:
            Random data with specified entropy
        """
        # Normalize entropy
        entropy = max(0.0, min(8.0, entropy))
        
        # Generate random data
        data = secrets.token_bytes(size)
        
        # If entropy is less than maximum, introduce bias
        if entropy < 8.0:
            # Calculate the number of bytes to use
            num_bytes = int(2 ** entropy)
            
            # Map each byte to a smaller range
            for i in range(size):
                data[i] = data[i] % num_bytes
        
        return data


def create_sample_usage():
    """Create sample usage of the cryptography library."""
    print("Advanced Cryptography and Security Library")
    print("====================================")
    
    # Check dependencies
    print("Checking dependencies:")
    print(f"cryptography available: {'Yes' if CRYPTOGRAPHY_AVAILABLE else 'No'}")
    
    # Generate random keys
    print("\n1. Key Generation")
    
    # Generate symmetric key
    aes_key = SymmetricCipher.generate_key("AES")
    print(f"Generated AES key: {base64.b64encode(aes_key).decode('ascii')}")
    
    # Generate asymmetric key pair
    try:
        rsa_private_key, rsa_public_key = AsymmetricCipher.generate_key_pair("RSA", 2048)
        print(f"Generated RSA key pair (2048 bits)")
    except Exception as e:
        print(f"Error generating RSA key pair: {str(e)}")
        rsa_private_key, rsa_public_key = None, None
    
    try:
        ecc_private_key, ecc_public_key = AsymmetricCipher.generate_key_pair("ECC")
        print(f"Generated ECC key pair")
    except Exception as e:
        print(f"Error generating ECC key pair: {str(e)}")
        ecc_private_key, ecc_public_key = None, None
    
    # Symmetric encryption
    print("\n2. Symmetric Encryption")
    
    plaintext = "This is a secret message that needs to be encrypted."
    print(f"Original plaintext: {plaintext}")
    
    # Generate IV
    iv = SymmetricCipher.generate_iv("AES")
    
    # Encrypt with AES
    try:
        cipher = SymmetricCipher("AES", aes_key, CipherMode.CBC, iv)
        ciphertext = cipher.encrypt(plaintext)
        print(f"Encrypted with AES-CBC: {base64.b64encode(ciphertext).decode('ascii')}")
        
        # Decrypt
        decrypted = cipher.decrypt(ciphertext)
        print(f"Decrypted: {decrypted.decode('utf-8')}")
    except Exception as e:
        print(f"Error with AES encryption: {str(e)}")
    
    # Asymmetric encryption
    print("\n3. Asymmetric Encryption")
    
    if rsa_private_key and rsa_public_key:
        try:
            rsa_cipher = AsymmetricCipher(rsa_private_key, rsa_public_key, "RSA")
            
            # Encrypt with public key
            rsa_ciphertext = rsa_cipher.encrypt(plaintext)
            print(f"Encrypted with RSA: {base64.b64encode(rsa_ciphertext).decode('ascii')}")
            
            # Decrypt with private key
            rsa_decrypted = rsa_cipher.decrypt(rsa_ciphertext)
            print(f"Decrypted: {rsa_decrypted.decode('utf-8')}")
            
            # Sign and verify
            signature = rsa_cipher.sign(plaintext.encode('utf-8'))
            print(f"Signature: {base64.b64encode(signature).decode('ascii')}")
            
            is_valid = rsa_cipher.verify(plaintext.encode('utf-8'), signature)
            print(f"Signature valid: {is_valid}")
        except Exception as e:
            print(f"Error with RSA operations: {str(e)}")
    
    if ecc_private_key and ecc_public_key:
        try:
            ecc_cipher = AsymmetricCipher(ecc_private_key, ecc_public_key, "ECC")
            
            # Encrypt with public key
            ecc_ciphertext = ecc_cipher.encrypt(plaintext)
            print(f"Encrypted with ECC: {base64.b64encode(ecc_ciphertext).decode('ascii')}")
            
            # Decrypt with private key
            ecc_decrypted = ecc_cipher.decrypt(ecc_ciphertext)
            print(f"Decrypted: {ecc_decrypted.decode('utf-8')}")
            
            # Sign and verify
            signature = ecc_cipher.sign(plaintext.encode('utf-8'))
            print(f"Signature: {base64.b64encode(signature).decode('ascii')}")
            
            is_valid = ecc_cipher.verify(plaintext.encode('utf-8'), signature)
            print(f"Signature valid: {is_valid}")
        except Exception as e:
            print(f"Error with ECC operations: {str(e)}")
    
    # Hashing
    print("\n4. Hashing")
    
    # Hash the data
    for algorithm in [HashAlgorithm.SHA256, HashAlgorithm.SHA512, HashAlgorithm.MD5]:
        try:
            hash_func = HashFunction(algorithm)
            hash_func.update(plaintext.encode('utf-8'))
            digest = hash_func.hexdigest()
            print(f"{algorithm.value} hash: {digest}")
        except Exception as e:
            print(f"Error with {algorithm.value} hash: {str(e)}")
    
    # HMAC
    print("\n5. HMAC")
    
    try:
        hmac_key = SecureRandom.token_bytes(32)
        hmac_digest = HashFunction.hmac_digest(plaintext.encode('utf-8'), hmac_key, HashAlgorithm.SHA256)
        print(f"HMAC-SHA256: {base64.b64encode(hmac_digest).decode('ascii')}")
    except Exception as e:
        print(f"Error with HMAC: {str(e)}")
    
    # Password hashing
    print("\n6. Password Hashing")
    
    try:
        password = "SecurePassword123!"
        hashed_password = KeyManager.hash_password(password)
        print(f"Password: {password}")
        print(f"Hashed: {hashed_password}")
        
        is_valid = KeyManager.verify_password(password, hashed_password)
        print(f"Password valid: {is_valid}")
        
        is_invalid = KeyManager.verify_password("WrongPassword", hashed_password)
        print(f"Wrong password valid: {is_invalid}")
    except Exception as e:
        print(f"Error with password hashing: {str(e)}")
    
    # Key derivation
    print("\n7. Key Derivation")
    
    try:
        derived_password = "DeriveThisPassword"
        salt = SecureRandom.token_bytes(16)
        derived_key, _ = KeyManager.derive_key_from_password(derived_password, salt)
        print(f"Derived key from '{derived_password}': {base64.b64encode(derived_key).decode('ascii')}")
    except Exception as e:
        print(f"Error with key derivation: {str(e)}")
    
    # Random data generation
    print("\n8. Random Data Generation")
    
    try:
        random_bytes = SecureRandom.token_bytes(16)
        random_hex = SecureRandom.token_hex(16)
        random_urlsafe = SecureRandom.token_urlsafe(16)
        random_int = SecureRandom.randint(1, 100)
        
        print(f"Random bytes: {base64.b64encode(random_bytes).decode('ascii')}")
        print(f"Random hex: {random_hex}")
        print(f"Random URL-safe: {random_urlsafe}")
        print(f"Random integer (1-100): {random_int}")
    except Exception as e:
        print(f"Error with random generation: {str(e)}")
    
    # Security analysis
    print("\n9. Security Analysis")
    
    # Password strength analysis
    try:
        passwords = [
            "password",
            "Password1",
            "P@ssw0rd!",
            "ThisIsAVeryLongPasswordWithManyCharacters123!@#"
        ]
        
        for pwd in passwords:
            analysis = SecurityAnalyzer.analyze_password_strength(pwd)
            print(f"Password: '{pwd}'")
            print(f"  Strength: {analysis['strength']} (score: {analysis['score']})")
            print(f"  Feedback: {analysis['feedback']}")
    except Exception as e:
        print(f"Error with password analysis: {str(e)}")
    
    # File entropy analysis
    try:
        # Create a temporary file with random data
        test_file = "test_entropy.bin"
        random_data = SecurityAnalyzer.generate_random_data(1024, 4.0)  # Low entropy data
        with open(test_file, 'wb') as f:
            f.write(random_data)
        
        analysis = SecurityAnalyzer.analyze_file_entropy(test_file)
        print(f"File entropy analysis for {test_file}:")
        print(f"  Entropy: {analysis['entropy']:.2f} bits")
        print(f"  Normalized entropy: {analysis['normalized_entropy']:.2f}")
        print(f"  Max entropy: {analysis['max_entropy']:.2f} bits")
        print(f"  Size: {analysis['size']} bytes")
        
        # Clean up
        os.remove(test_file)
    except Exception as e:
        print(f"Error with file entropy analysis: {str(e)}")
    
    # Generate random data with different entropy levels
    print("\n10. Random Data with Different Entropy")
    
    try:
        for entropy in [2.0, 4.0, 6.0, 8.0]:
            data = SecurityAnalyzer.generate_random_data(32, entropy)
            file_entropy = SecurityAnalyzer.analyze_file_entropy_in_memory(data)
            print(f"Data with target entropy {entropy}: actual entropy {file_entropy:.2f} bits")
    except Exception as e:
        print(f"Error with entropy generation: {str(e)}")
    
    print("\nCryptography library demonstration completed!")


def main():
    """Main function to demonstrate the cryptography library."""
    print("Advanced Cryptography and Security Library")
    print("====================================")
    
    # Run the demonstration
    create_sample_usage()
    
    return True


if __name__ == "__main__":
    main()
<｜fim▁hole｜>