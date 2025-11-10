/**
 * Blockchain and Cryptocurrency System
 * 
 * This module implements a simplified blockchain and cryptocurrency system
 * that can handle transactions, wallets, and basic mining simulation.
 * 
 * Features:
 * - Block structure and validation
 * - Transaction creation and validation
 * - Wallet management with multiple accounts
 * - Mining simulation
 * - Peer-to-peer networking
 * - Consensus mechanism
 * - Cryptocurrency transfer
 */

// Block class represents a single block in the blockchain
class Block {
    constructor(index, previousHash, timestamp, data, merkleRoot = null) {
        this.index = index;
        this.previousHash = previousHash;
        this.timestamp = timestamp;
        this.data = data;
        this.merkleRoot = merkleRoot || this.calculateMerkleRoot(data);
        this.nonce = Math.floor(Math.random() * 100000000);
        this.hash = this.calculateHash();
    }
    
    /**
     * Calculate block hash based on all block properties
     */
    calculateHash() {
        const dataString = `${this.index}${this.previousHash}${this.timestamp}${JSON.stringify(this.data)}${this.merkleRoot || ''}${this.nonce}`;
        return this.hashFunction(dataString);
    }
    
    /**
     * Calculate Merkle root from transactions
     * @param {Array} transactions - Transaction data
     */
    calculateMerkleRoot(transactions) {
        if (transactions.length === 0) return '';
        
        // Simple Merkle tree implementation
        const leaves = transactions.map(tx => this.hashFunction(JSON.stringify(tx)));
        
        return this.merkleRootRecursive(leaves);
    }
    
    /**
     * Recursive Merkle root calculation
     * @param {Array} nodes - Array of node hashes
     */
    merkleRootRecursive(nodes) {
        if (nodes.length === 1) {
            return nodes[0];
        }
        
        const newLevel = [];
        for (let i = 0; i < nodes.length; i += 2) {
            if (i + 1 < nodes.length) {
                newLevel.push(this.hashFunction(nodes[i] + nodes[i + 1]));
            } else {
                newLevel.push(nodes[i]);
            }
        }
        
        return this.merkleRootRecursive(newLevel);
    }
    
    /**
     * Simple hash function (simplified for demonstration)
     * @param {string} data - Data to hash
     */
    hashFunction(data) {
        let hash = 0;
        
        for (let i = 0; i < data.length; i++) {
            const char = data.charCodeAt(i);
            hash = ((hash << 5) - hash) + char;
            hash = hash & hash;
            hash = ((hash << 3) - hash) + char;
        }
        
        return Math.abs(hash).toString(16);
    }
    
    /**
     * Validate block against previous block
     * @param {Block} previousBlock - Previous block in chain
     */
    validate(previousBlock) {
        // Check index
        if (this.index !== previousBlock.index + 1) {
            return false;
        }
        
        // Check previous hash
        if (this.previousHash !== previousBlock.hash) {
            return false;
        }
        
        // Check timestamp (must be later than previous)
        if (this.timestamp <= previousBlock.timestamp) {
            return false;
        }
        
        // Validate hash
        if (this.hash !== this.calculateHash()) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Get block summary
     */
    getSummary() {
        return {
            index: this.index,
            hash: this.hash,
            previousHash: this.previousHash,
            timestamp: this.timestamp,
            data: this.data,
            merkleRoot: this.merkleRoot,
            nonce: this.nonce,
            transactionCount: this.data.transactions ? this.data.transactions.length : 0
        };
    }
}

// Transaction class represents a blockchain transaction
class Transaction {
    constructor(fromAddress, toAddress, amount, data = null, signature = null) {
        this.id = this.generateId();
        this.fromAddress = fromAddress;
        this.toAddress = toAddress;
        this.amount = amount;
        this.data = data;
        this.timestamp = new Date();
        this.signature = signature;
        this.confirmed = false;
        this.confirmations = [];
        this.blockIndex = null;
    }
    
    /**
     * Generate unique transaction ID
     */
    generateId() {
        return Date.now().toString(36) + Math.random().toString(36).substring(2);
    }
    
    /**
     * Calculate transaction fee
     */
    getFee() {
        // Simplified fee calculation based on transaction size
        const dataSize = JSON.stringify(this.data).length;
        return Math.max(0.001, dataSize * 0.00001); // Minimum 0.001 and 0.00001 per byte
    }
    
    /**
     * Sign transaction with private key
     * @param {string} privateKey - Private key for signing
     */
    sign(privateKey) {
        if (!privateKey) {
            throw new Error('Private key is required for signing');
        }
        
        // Create message to sign
        const message = this.createMessage();
        
        // Simplified signing (in real implementation, use crypto libraries)
        // This is just for demonstration
        const signature = this.simplifiedSign(message, privateKey);
        this.signature = signature;
    }
    
    /**
     * Create message for signing
     */
    createMessage() {
        return `${this.fromAddress}${this.toAddress}${this.amount}${JSON.stringify(this.data)}${this.timestamp.getTime()}`;
    }
    
    /**
     * Simplified signing (for demonstration purposes only)
     * @param {string} message - Message to sign
     * @param {string} privateKey - Private key
     */
    simplifiedSign(message, privateKey) {
        // In a real implementation, this would use cryptographic algorithms
        // This is just a placeholder
        return `signed_${message}_with_${privateKey}`;
    }
    
    /**
     * Verify transaction signature
     * @param {string} publicKey - Public key
     */
    verify(publicKey) {
        if (!this.signature || !publicKey) {
            return false;
        }
        
        // Create message to verify
        const message = this.createMessage();
        
        // Simplified verification (for demonstration purposes only)
        // This would use cryptographic verification in a real implementation
        return this.signature === `signed_${message}_with_${publicKey}`;
    }
    
    /**
     * Confirm transaction
     * @param {string} nodeId - Node ID confirming the transaction
     */
    confirm(nodeId) {
        this.confirmed = true;
        this.confirmations.push({
            nodeId: nodeId,
            timestamp: new Date()
        });
    }
    
    /**
     * Get transaction summary
     */
    getSummary() {
        return {
            id: this.id,
            fromAddress: this.fromAddress,
            toAddress: this.toAddress,
            amount: this.amount,
            data: this.data,
            timestamp: this.timestamp,
            fee: this.getFee(),
            signature: this.signature,
            confirmed: this.confirmed,
            confirmations: this.confirmations,
            blockIndex: this.blockIndex
        };
    }
}

// Wallet class represents a cryptocurrency wallet
class Wallet {
    constructor(privateKey, publicKey, address) {
        this.privateKey = privateKey;
        this.publicKey = publicKey;
        this.address = address;
        this.balance = 0;
        this.transactions = [];
        this.utxos = [];  // Unspent Transaction Outputs
        this.transactionPool = new Map(); // Mempool
        this.network = null;
    }
    
    /**
     * Get address
     */
    getAddress() {
        return this.address;
    }
    
    /**
     * Get balance
     */
    getBalance() {
        return this.balance;
    }
    
    /**
     * Get UTXOs
     */
    getUtxos() {
        return this.utxos;
    }
    
    /**
     * Add UTXO
     * @param {Object} utxo - Unspent Transaction Output
     */
    addUtxo(utxo) {
        this.utxos.push(utxo);
    }
    
    /**
     * Create and sign transaction
     * @param {string} toAddress - Recipient address
     * @param {number} amount - Amount to send
     * @param {Object} data - Transaction data
     */
    createTransaction(toAddress, amount, data = null) {
        // Calculate fee
        const fee = new Transaction().getFee();
        
        // Calculate total amount needed (amount + fee)
        const totalAmount = amount + fee;
        
        // Check if we have enough balance
        if (this.balance < totalAmount) {
            throw new Error(`Insufficient balance. Required: ${totalAmount}, Available: ${this.balance}`);
        }
        
        // Create transaction
        const transaction = new Transaction(
            this.address,
            toAddress,
            amount,
            data
        );
        
        // Sign transaction
        transaction.sign(this.privateKey);
        
        return transaction;
    }
    
    /**
     * Add transaction to network transaction pool
     * @param {Transaction} transaction - Transaction to add
     * @param {Network} network - Network to propagate to
     */
    addTransactionToPool(transaction, network) {
        this.network = network;
        this.transactionPool.set(transaction.id, transaction);
        
        // Add to wallet transactions
        this.transactions.push(transaction);
        
        // Notify network
        network.propagateTransaction(transaction);
    }
    
    /**
     * Get transactions
     */
    getTransactions() {
        return this.transactions;
    }
    
    /**
     * Get pending transactions
     */
    getPendingTransactions() {
        return Array.from(this.transactionPool.values()).filter(tx => !tx.confirmed);
    }
    
    /**
     * Update balance after block
     * @param {Block} block - New block
     */
    updateBalance(block) {
        // Reset transaction pool
        this.transactionPool.clear();
        
        // Update balance for incoming and outgoing transactions
        for (const transaction of block.data.transactions) {
            if (transaction.toAddress === this.address) {
                // Incoming transaction
                this.balance += transaction.amount - transaction.getFee();
            } else if (transaction.fromAddress === this.address) {
                // Outgoing transaction
                this.balance -= transaction.amount;
            }
        }
        
        // Update confirmed status for transactions in block
        for (const transaction of block.data.transactions) {
            const tx = this.transactions.find(t => t.id === transaction.id);
            if (tx) {
                tx.blockIndex = block.index;
            }
        }
    }
    
    /**
     * Mine a new block
     * @param {Network} network - Network to mine on
     */
    mineBlock(network) {
        this.network = network;
        
        // Get pending transactions from pool
        const pendingTransactions = network.getPendingTransactions();
        const validTransactions = pendingTransactions.filter(tx => 
            this.network.validateTransaction(tx)
        );
        
        if (validTransactions.length === 0) {
            throw new Error('No valid transactions to include in block');
        }
        
        // Sort transactions by fee (highest first)
        validTransactions.sort((a, b) => b.getFee() - a.getFee());
        
        // Create block data
        const blockData = {
            transactions: validTransactions
        };
        
        // Get previous block
        const previousBlock = network.getLatestBlock();
        const previousHash = previousBlock ? previousBlock.hash : '';
        
        // Create new block
        const block = new Block(
            network.getLatestBlock() ? network.getLatestBlock().index + 1 : 0,
            previousHash,
            new Date(),
            blockData
        );
        
        // Validate block
        if (previousBlock && !block.validate(previousBlock)) {
            throw new Error('Block validation failed');
        }
        
        // Add block to network
        network.addBlock(block);
        
        return block;
    }
}

// Network class simulates peer-to-peer networking
class Network {
    constructor() {
        this.nodes = [];
        this.blockchain = [];
        this.pendingTransactions = new Map();
        this.difficulty = 1;
        this.miningReward = 10.0;
        this.consensusAlgorithm = 'proofOfWork';
    }
    
    /**
     * Add node to network
     * @param {Node} node - Node to add
     */
    addNode(node) {
        this.nodes.push(node);
        node.joinNetwork(this);
    }
    
    /**
     * Get blockchain
     */
    getBlockchain() {
        return this.blockchain;
    }
    
    /**
     * Get latest block
     */
    getLatestBlock() {
        return this.blockchain.length > 0 ? this.blockchain[this.blockchain.length - 1] : null;
    }
    
    /**
     * Add block to blockchain
     * @param {Block} block - Block to add
     */
    addBlock(block) {
        // Validate block
        if (!this.isValidBlock(block)) {
            throw new Error('Invalid block');
        }
        
        // Add to blockchain
        this.blockchain.push(block);
        
        // Update all nodes with new block
        this.nodes.forEach(node => {
            node.updateBlockchain(block);
        });
        
        // Update pending transactions
        this.updatePendingTransactions(block);
    }
    
    /**
     * Validate block
     * @param {Block} block - Block to validate
     */
    isValidBlock(block) {
        // Check if previous hash matches
        const expectedPreviousHash = block.index === 0 ? '' : 
            this.blockchain[block.index - 1] ? this.blockchain[block.index - 1].hash : '';
        
        if (block.previousHash !== expectedPreviousHash) {
            return false;
        }
        
        // Check if block hash is valid
        if (block.hash !== block.calculateHash()) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Add transaction to pending pool
     * @param {Transaction} transaction - Transaction to add
     */
    addPendingTransaction(transaction) {
        // Validate transaction
        if (!this.validateTransaction(transaction)) {
            throw new Error('Invalid transaction');
        }
        
        this.pendingTransactions.set(transaction.id, transaction);
    }
    
    /**
     * Get pending transactions
     */
    getPendingTransactions() {
        return Array.from(this.pendingTransactions.values());
    }
    
    /**
     * Update pending transactions after block
     * @param {Block} block - New block
     */
    updatePendingTransactions(block) {
        // Remove transactions included in the new block
        for (const transaction of block.data.transactions) {
            this.pendingTransactions.delete(transaction.id);
        }
    }
    
    /**
     * Propagate transaction to network
     * @param {Transaction} transaction - Transaction to propagate
     */
    propagateTransaction(transaction) {
        // Share transaction with all nodes
        this.nodes.forEach(node => {
            node.receiveTransaction(transaction);
        });
    }
    
    /**
     * Validate transaction
     * @param {Transaction} transaction - Transaction to validate
     */
    validateTransaction(transaction) {
        // Check signature
        if (!transaction.verify(transaction.fromAddress)) {
            return false;
        }
        
        // Check if transaction conflicts with existing ones
        const conflicts = Array.from(this.pendingTransactions.values()).filter(tx => 
            (tx.fromAddress === transaction.fromAddress && tx.toAddress === transaction.toAddress) ||
            tx.fromAddress === transaction.toAddress && tx.toAddress === transaction.fromAddress)
        );
        
        return conflicts.length === 0;
    }
    
    /**
     * Achieve consensus
     */
    achieveConsensus() {
        // Simplified consensus mechanism
        // In a real implementation, this would use a consensus algorithm like Proof of Work
        return true;
    }
}

// Node class represents a network node
class Node {
    constructor(id, address, isMiner = false) {
        this.id = id;
        this.address = address;
        this.isMiner = isMiner;
        this.blockchain = [];
        this.peers = [];
        this.network = null;
    }
    
    /**
     * Join network
     * @param {Network} network - Network to join
     */
    joinNetwork(network) {
        this.network = network;
        
        // Get blockchain from network
        this.blockchain = network.getBlockchain();
        
        // Connect to all other nodes (simplified)
        this.peers = network.nodes.filter(node => node.id !== this.id);
    }
    
    /**
     * Update blockchain
     * @param {Block} block - New block to add
     */
    updateBlockchain(block) {
        this.blockchain.push(block);
    }
    
    /**
     * Receive transaction
     * @param {Transaction} transaction - Received transaction
     */
    receiveTransaction(transaction) {
        console.log(`Node ${this.id} received transaction: ${transaction.id}`);
        
        // Validate transaction
        if (this.network.validateTransaction(transaction)) {
            // Add to pending transactions
            this.network.addPendingTransaction(transaction);
        }
    }
    
    /**
     * Start mining (for miner nodes)
     */
    startMining() {
        if (!this.isMiner) {
            throw new Error('This node is not configured for mining');
        }
        
        // Start mining process (simplified)
        const mineBlock = () => {
            try {
                // Mine a new block
                const block = this.network.mineBlock();
                console.log(`Node ${this.id} mined block: ${block.index}`);
                
                // Continue mining
                setTimeout(mineBlock, Math.random() * 10000);
            } catch (error) {
                console.error(`Mining error on node ${this.id}:`, error.message);
            }
        };
        
        mineBlock();
    }
    
    /**
     * Get node summary
     */
    getSummary() {
        return {
            id: this.id,
            address: this.address,
            isMiner: this.isMiner,
            blockchain: this.blockchain.length > 0 ? this.blockchain[this.blockchain.length - 1].getSummary() : null,
            peerCount: this.peers.length,
            networkDifficulty: this.network ? this.network.difficulty : 0
        };
    }
}

// Blockchain class ties everything together
class Blockchain {
    constructor() {
        this.network = new Network();
        this.wallets = new Map();
        this.genesisBlock = null;
        this.isInitialized = false;
    }
    
    /**
     * Initialize the blockchain
     */
    initialize() {
        if (this.isInitialized) {
            throw new Error('Blockchain is already initialized');
        }
        
        // Create genesis block
        this.genesisBlock = new Block(
            0,
            '',
            new Date(),
            {
                transactions: []
            }
        );
        
        // Add genesis block to network
        this.network.addBlock(this.genesisBlock);
        
        this.isInitialized = true;
        return this.genesisBlock;
    }
    
    /**
     * Create a new wallet
     * @param {number} balance - Initial balance
     */
    createWallet(balance = 0) {
        // Generate key pair (simplified)
        const privateKey = 'private_' + Date.now().toString(36);
        const publicKey = 'public_' + privateKey;
        const address = 'address_' + privateKey;
        
        const wallet = new Wallet(privateKey, publicKey, address);
        wallet.balance = balance;
        
        // Add wallet to blockchain
        this.wallets.set(address, wallet);
        wallet.network = this.network;
        
        return wallet;
    }
    
    /**
     * Get wallet by address
     * @param {string} address - Wallet address
     */
    getWallet(address) {
        return this.wallets.get(address);
    }
    
    /**
     * Create a transaction
     * @param {Wallet} wallet - Sender wallet
     * @param {string} toAddress - Recipient address
     * @param {number} amount - Amount to send
     * @param {Object} data - Transaction data
     */
    createTransaction(fromAddress, toAddress, amount, data = null) {
        const wallet = this.getWallet(fromAddress);
        if (!wallet) {
            throw new Error(`Wallet not found for address: ${fromAddress}`);
        }
        
        const transaction = wallet.createTransaction(toAddress, amount, data);
        return transaction;
    }
    
    /**
     * Add transaction to network
     * @param {Transaction} transaction - Transaction to add
     */
    addTransaction(transaction) {
        const wallet = this.getWallet(transaction.fromAddress);
        if (!wallet) {
            throw new Error(`Wallet not found for address: ${transaction.fromAddress}`);
        }
        
        wallet.addTransactionToPool(transaction, this.network);
    }
    
    /**
     * Add node to network
     * @param {boolean} isMiner - Whether node is a miner
     */
    addNode(isMiner = false) {
        const id = 'node_' + Date.now().toString(36);
        const address = 'address_' + id;
        
        const node = new Node(id, address, isMiner);
        this.network.addNode(node);
        
        return node;
    }
    
    /**
     * Mine a new block
     * @param {string} nodeId - Node ID to mine
     */
    mineBlock(nodeId) {
        const node = this.network.nodes.find(n => n.id === nodeId);
        if (!node) {
            throw new Error(`Node not found with ID: ${nodeId}`);
        }
        
        if (!node.isMiner) {
            throw new Error(`Node ${nodeId} is not a miner`);
        }
        
        return node.mineBlock();
    }
    
    /**
     * Get network summary
     */
    getNetworkSummary() {
        const totalTransactions = this.network.pendingTransactions.size;
        const totalNodes = this.network.nodes.length;
        const totalWallets = this.wallets.size;
        const totalBalance = Array.from(this.wallets.values()).reduce((total, wallet) => total + wallet.getBalance(), 0);
        const latestBlock = this.network.getLatestBlock();
        
        return {
            networkStatus: 'active',
            blockchainLength: this.network.blockchain.length,
            pendingTransactions: totalTransactions,
            totalNodes: totalNodes,
            totalWallets: totalWallets,
            totalBalance,
            latestBlock: latestBlock ? latestBlock.getSummary() : null
        };
    }
    
    /**
     * Get all wallets
     */
    getAllWallets() {
        return Array.from(this.wallets.values());
    }
    
    /**
     * Get all nodes
     */
    getAllNodes() {
        return this.network.nodes;
    }
    
    /**
     * Get blockchain
     */
    getBlockchain() {
        return this.network.blockchain;
    }
    
    /**
     * Get transaction by ID
     * @param {string} transactionId - Transaction ID
     */
    getTransaction(transactionId) {
        // Search all wallets for the transaction
        for (const wallet of this.getAllWallets()) {
            const transactions = wallet.getTransactions();
            const transaction = transactions.find(t => t.id === transactionId);
            if (transaction) {
                return transaction;
            }
        }
        
        return null;
    }
}

// MiningPool class manages mining operations across multiple miners
class MiningPool {
    constructor(blockchain) {
        this.blockchain = blockchain;
        this.miners = [];
        this.blocksMined = 0;
        this.totalRewards = 0;
        this.isRunning = false;
    }
    
    /**
     * Add miner to pool
     * @param {string} minerId - Miner ID
     */
    addMiner(minerId) {
        this.miners.push(minerId);
    }
    
    /**
     * Remove miner from pool
     * @param {string} minerId - Miner ID
     */
    removeMiner(minerId) {
        const index = this.miners.indexOf(minerId);
        if (index > -1) {
            this.miners.splice(index, 1);
        }
    }
    
    /**
     * Start mining pool
     */
    start() {
        if (this.isRunning) {
            throw new Error('Mining pool is already running');
        }
        
        if (this.miners.length === 0) {
            throw new Error('No miners in the pool');
        }
        
        this.isRunning = true;
        this.scheduleNextBlock();
    }
    
    /**
     * Stop mining pool
     */
    stop() {
        this.isRunning = false;
    }
    
    /**
     * Schedule next block mining
     */
    scheduleNextBlock() {
        if (!this.isRunning) return;
        
        // Random delay between blocks
        const delay = Math.random() * 30000 + 10000; // 10-40 seconds
        
        setTimeout(() => {
            this.mineNextBlock();
        }, delay);
    }
    
    /**
     * Mine next block
     */
    mineNextBlock() {
        if (!this.isRunning) return;
        
        // Select random miner
        const minerIndex = Math.floor(Math.random() * this.miners.length);
        const minerId = this.miners[minerIndex];
        
        try {
            // Mine block
            const block = this.blockchain.mineBlock(minerId);
            
            // Update statistics
            this.blocksMined++;
            this.totalRewards += this.blockchain.network.miningReward;
            
            console.log(`Block ${block.index} mined by ${minerId}`);
            
            // Schedule next block
            this.scheduleNextBlock();
        } catch (error) {
            console.error(`Mining error: ${error.message}`);
            
            // Schedule next block with shorter delay
            setTimeout(() => {
                this.scheduleNextBlock();
            }, 5000);
        }
    }
    
    /**
     * Get pool statistics
     */
    getStatistics() {
        return {
            isRunning: this.isRunning,
            minersCount: this.miners.length,
            blocksMined: this.blocksMined,
            totalRewards: this.totalRewards,
            averageReward: this.blocksMined > 0 ? this.totalRewards / this.blocksMined : 0
        };
    }
}

// BlockchainAnalytics class provides analytics and monitoring for blockchain
class BlockchainAnalytics {
    constructor(blockchain) {
        this.blockchain = blockchain;
        this.metrics = {
            totalTransactions: 0,
            totalBlocks: 0,
            totalAmount: 0,
            averageBlockSize: 0,
            averageTransactionsPerBlock: 0,
            networkHashrate: 0,
            difficulty: blockchain.network.difficulty
        };
        
        this.updateMetrics();
    }
    
    /**
     * Update analytics metrics
     */
    updateMetrics() {
        const blockchain = this.blockchain.getBlockchain();
        let totalTransactions = 0;
        let totalAmount = 0;
        
        for (const block of blockchain) {
            const blockTransactions = block.data.transactions || [];
            totalTransactions += blockTransactions.length;
            
            for (const tx of blockTransactions) {
                totalAmount += tx.amount;
            }
        }
        
        this.metrics.totalTransactions = totalTransactions;
        this.metrics.totalBlocks = blockchain.length;
        this.metrics.totalAmount = totalAmount;
        
        if (blockchain.length > 1) {
            const totalSize = blockchain.reduce((sum, block) => 
                sum + JSON.stringify(block).length, 0);
            this.metrics.averageBlockSize = totalSize / blockchain.length;
            this.metrics.averageTransactionsPerBlock = totalTransactions / blockchain.length;
        }
        
        // Update network metrics
        this.metrics.difficulty = this.blockchain.network.difficulty;
        
        // Simple hashrate calculation (blocks per minute)
        const recentBlocks = blockchain.slice(-10);
        if (recentBlocks.length >= 2) {
            const timeSpan = recentBlocks[recentBlocks.length - 1].timestamp - recentBlocks[0].timestamp;
            this.metrics.networkHashrate = (recentBlocks.length / timeSpan) * 60000; // per minute
        }
    }
    
    /**
     * Get transaction by address
     * @param {string} address - Wallet address
     */
    getTransactionsByAddress(address) {
        const blockchain = this.blockchain.getBlockchain();
        const transactions = [];
        
        for (const block of blockchain) {
            const blockTransactions = block.data.transactions || [];
            for (const tx of blockTransactions) {
                if (tx.fromAddress === address || tx.toAddress === address) {
                    transactions.push(tx);
                }
            }
        }
        
        return transactions;
    }
    
    /**
     * Get transaction volume by time period
     * @param {Date} startDate - Start date
     * @param {Date} endDate - End date
     */
    getTransactionVolumeByTimePeriod(startDate, endDate) {
        const blockchain = this.blockchain.getBlockchain();
        const transactions = [];
        
        for (const block of blockchain) {
            if (block.timestamp >= startDate && block.timestamp <= endDate) {
                const blockTransactions = block.data.transactions || [];
                for (const tx of blockTransactions) {
                    transactions.push(tx);
                }
            }
        }
        
        return {
            transactionCount: transactions.length,
            totalAmount: transactions.reduce((sum, tx) => sum + tx.amount, 0),
            averageAmount: transactions.length > 0 ? 
                transactions.reduce((sum, tx) => sum + tx.amount, 0) / transactions.length : 0,
            transactions
        };
    }
    
    /**
     * Get address balance
     * @param {string} address - Wallet address
     */
    getAddressBalance(address) {
        const wallet = this.blockchain.getWallet(address);
        if (wallet) {
            return wallet.getBalance();
        }
        
        // Calculate balance from blockchain if wallet not found
        const transactions = this.getTransactionsByAddress(address);
        let balance = 0;
        
        for (const tx of transactions) {
            if (tx.toAddress === address) {
                balance += tx.amount - tx.getFee();
            } else if (tx.fromAddress === address) {
                balance -= tx.amount;
            }
        }
        
        return balance;
    }
    
    /**
     * Get top addresses by balance
     * @param {number} limit - Maximum number of addresses to return
     */
    getTopAddressesByBalance(limit = 10) {
        const addresses = [];
        
        // Get all wallets
        const wallets = this.blockchain.getAllWallets();
        for (const wallet of wallets) {
            addresses.push({
                address: wallet.getAddress(),
                balance: wallet.getBalance()
            });
        }
        
        // Sort by balance
        addresses.sort((a, b) => b.balance - a.balance);
        
        return addresses.slice(0, limit);
    }
    
    /**
     * Get analytics summary
     */
    getAnalyticsSummary() {
        this.updateMetrics();
        
        return {
            blockchain: this.metrics,
            network: {
                nodes: this.blockchain.network.nodes.length,
                pendingTransactions: this.blockchain.network.pendingTransactions.size,
                miningReward: this.blockchain.network.miningReward
            },
            wallets: {
                total: this.blockchain.getAllWallets().length,
                totalBalance: this.blockchain.getAllWallets().reduce(
                    (sum, wallet) => sum + wallet.getBalance(), 0
                )
            }
        };
    }
}

// SmartContract class represents a blockchain smart contract
class SmartContract {
    constructor(address, code, owner) {
        this.address = address;
        this.code = code;
        this.owner = owner;
        this.balance = 0;
        this.storage = {};
        this.transactions = [];
        this.state = 'deployed';
        this.creationTime = new Date();
    }
    
    /**
     * Execute contract method
     * @param {string} method - Method name
     * @param {Array} args - Method arguments
     * @param {string} caller - Caller address
     */
    execute(method, args, caller) {
        if (!this.code[method]) {
            throw new Error(`Method ${method} not found in contract`);
        }
        
        // Record transaction
        const tx = {
            method: method,
            args: args,
            caller: caller,
            timestamp: new Date(),
            gasUsed: this.estimateGas(method, args),
            success: false,
            result: null,
            error: null
        };
        
        try {
            // Execute method
            const result = this.code[method](this.storage, args, caller);
            tx.success = true;
            tx.result = result;
            
            // Record successful transaction
            this.transactions.push(tx);
            
            return result;
        } catch (error) {
            tx.error = error.message;
            this.transactions.push(tx);
            
            throw error;
        }
    }
    
    /**
     * Estimate gas cost for method execution
     * @param {string} method - Method name
     * @param {Array} args - Method arguments
     */
    estimateGas(method, args) {
        // Simplified gas estimation
        let gas = 21000; // Base gas cost
        
        // Add gas for each argument
        for (const arg of args) {
            if (typeof arg === 'string') {
                gas += arg.length * 200; // 200 gas per character for strings
            } else if (typeof arg === 'number') {
                gas += 500; // 500 gas for numbers
            } else if (typeof arg === 'object') {
                gas += JSON.stringify(arg).length * 100; // 100 gas per character for objects
            }
        }
        
        return gas;
    }
    
    /**
     * Send funds to contract
     * @param {number} amount - Amount to send
     */
    sendFunds(amount) {
        this.balance += amount;
    }
    
    /**
     * Get contract summary
     */
    getSummary() {
        return {
            address: this.address,
            owner: this.owner,
            balance: this.balance,
            state: this.state,
            creationTime: this.creationTime,
            transactionCount: this.transactions.length,
            storageSize: JSON.stringify(this.storage).length
        };
    }
    
    /**
     * Get transaction history
     */
    getTransactionHistory() {
        return this.transactions;
    }
}

// Main function to demonstrate blockchain and cryptocurrency system
function main() {
    console.log('Initializing Blockchain and Cryptocurrency System...\n');
    
    // Create and initialize blockchain
    const blockchain = new Blockchain();
    const genesisBlock = blockchain.initialize();
    console.log('Genesis block created:', genesisBlock.getSummary());
    
    // Create wallets
    const wallet1 = blockchain.createWallet(1000.0);
    const wallet2 = blockchain.createWallet(500.0);
    const wallet3 = blockchain.createWallet(750.0);
    
    console.log('Created wallets:');
    console.log(`Wallet 1: ${wallet1.getAddress()} - Balance: ${wallet1.getBalance()}`);
    console.log(`Wallet 2: ${wallet2.getAddress()} - Balance: ${wallet2.getBalance()}`);
    console.log(`Wallet 3: ${wallet3.getAddress()} - Balance: ${wallet3.getBalance()}`);
    
    // Add nodes to network
    const node1 = blockchain.addNode(true);  // Mining node
    const node2 = blockchain.addNode(false); // Regular node
    const node3 = blockchain.addNode(false); // Regular node
    
    console.log('Added nodes to network:');
    console.log(`Node 1: ${node1.id} - Miner: ${node1.isMiner}`);
    console.log(`Node 2: ${node2.id} - Miner: ${node2.isMiner}`);
    console.log(`Node 3: ${node3.id} - Miner: ${node3.isMiner}`);
    
    // Create mining pool
    const miningPool = new MiningPool(blockchain);
    miningPool.addMiner(node1.id);
    
    // Create analytics
    const analytics = new BlockchainAnalytics(blockchain);
    
    // Create and add some transactions
    try {
        console.log('Creating transactions...');
        
        // Transaction from wallet1 to wallet2
        const tx1 = blockchain.createTransaction(wallet1.getAddress(), wallet2.getAddress(), 250.0, { type: 'payment', note: 'Payment for services' });
        console.log(`Transaction 1: ${tx1.getSummary()}`);
        blockchain.addTransaction(tx1);
        
        // Transaction from wallet2 to wallet3
        const tx2 = blockchain.createTransaction(wallet2.getAddress(), wallet3.getAddress(), 500.0, { type: 'purchase', productId: 'prod_123', itemName: 'Digital Asset' });
        console.log(`Transaction 2: ${tx2.getSummary()}`);
        blockchain.addTransaction(tx2);
        
        // Transaction from wallet3 to wallet1
        const tx3 = blockchain.createTransaction(wallet3.getAddress(), wallet1.getAddress(), 100.0, { type: 'investment', projectId: 'proj_456' });
        console.log(`Transaction 3: ${tx3.getSummary()}`);
        blockchain.addTransaction(tx3);
        
        // Start mining pool
        console.log('Starting mining pool...');
        miningPool.start();
        
        // Display blockchain after some blocks
        setTimeout(() => {
            console.log('Blockchain after several blocks:');
            const updatedBlockchain = blockchain.getBlockchain();
            updatedBlockchain.forEach((block, index) => {
                console.log(`Block ${block.index}: ${block.hash} (Previous: ${block.previousHash})`);
                if (block.data.transactions.length > 0) {
                    console.log(`  Transactions: ${block.data.transactions.length}`);
                    for (const tx of block.data.transactions) {
                        console.log(`    - From: ${tx.fromAddress} To: ${tx.toAddress} Amount: ${tx.amount}`);
                    }
                }
            });
            
            // Get network summary
            const networkSummary = blockchain.getNetworkSummary();
            console.log('\nNetwork Summary:');
            console.log(`  Blockchain Length: ${networkSummary.blockchainLength}`);
            console.log(`  Pending Transactions: ${networkSummary.pendingTransactions}`);
            console.log(`  Total Nodes: ${networkSummary.totalNodes}`);
            console.log(`  Total Wallets: ${networkSummary.totalWallets}`);
            console.log(`  Total Balance: ${networkSummary.totalBalance}`);
            console.log(`  Latest Block: ${networkSummary.latestBlock ? networkSummary.latestBlock.index : 'None'}`);
            
            // Get analytics summary
            const analyticsSummary = analytics.getAnalyticsSummary();
            console.log('\nAnalytics Summary:');
            console.log(`  Total Transactions: ${analyticsSummary.blockchain.totalTransactions}`);
            console.log(`  Total Amount: ${analyticsSummary.blockchain.totalAmount}`);
            console.log(`  Average Block Size: ${analyticsSummary.blockchain.averageBlockSize}`);
            console.log(`  Average Transactions Per Block: ${analyticsSummary.blockchain.averageTransactionsPerBlock}`);
            console.log(`  Network Hashrate: ${analyticsSummary.blockchain.networkHashrate}`);
            console.log(`  Network Difficulty: ${analyticsSummary.blockchain.difficulty}`);
            
            // Get top addresses by balance
            const topAddresses = analytics.getTopAddressesByBalance();
            console.log('\nTop Addresses by Balance:');
            topAddresses.forEach((addr, index) => {
                console.log(`  ${index + 1}. ${addr.address}: ${addr.balance}`);
            });
            
            // Get mining pool statistics
            const miningStats = miningPool.getStatistics();
            console.log('\nMining Pool Statistics:');
            console.log(`  Is Running: ${miningStats.isRunning}`);
            console.log(`  Miners Count: ${miningStats.minersCount}`);
            console.log(`  Blocks Mined: ${miningStats.blocksMined}`);
            console.log(`  Total Rewards: ${miningStats.totalRewards}`);
            console.log(`  Average Reward: ${miningStats.averageReward}`);
        }, 30000);
        
    } catch (error) {
        console.error('Error in blockchain demonstration:', error.message);
    }
}

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        Block,
        Transaction,
        Wallet,
        Network,
        Node,
        Blockchain,
        MiningPool,
        BlockchainAnalytics,
        SmartContract
    };
}