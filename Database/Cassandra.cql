-- Create Keyspace
CREATE KEYSPACE IF NOT EXISTS bank_keyspace
WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};

-- Create User Table
CREATE TABLE IF NOT EXISTS bank_keyspace.user (
    username TEXT PRIMARY KEY,
    password TEXT,
    account_number TEXT,
    fullname TEXT,
    balance INT,
    age INT
);

-- Log in
SELECT * FROM bank_keyspace.user WHERE username = ? AND password = ?;

-- Get Account number (username)
SELECT account_number FROM bank_keyspace.user WHERE username = ?;

-- Get Account number (admin, username)
-- Assuming admin has access to all user data
SELECT account_number FROM bank_keyspace.user WHERE username = ?;

-- View Account balance(account number)
SELECT balance FROM bank_keyspace.user WHERE account_number = ?;

-- View transaction history(account number, count)
-- Assuming there's a separate table for transaction history, let's call it transaction_history
CREATE TABLE IF NOT EXISTS bank_keyspace.transaction_history (
    account_number TEXT,
    transaction_time TIMESTAMP,
    transaction_amount INT,
    PRIMARY KEY (account_number, transaction_time)
);

-- Make Transaction (account number, transaction amount)
-- This would involve updating the balance in the user table and inserting a record into the transaction_history table

-- Make Transfer (from account number, to account number, transfer amount)
-- This would involve updating balances for both from and to account numbers and inserting records into the transaction_history table

-- View Bank Database
SELECT * FROM bank_keyspace.user;

-- Create New User (userdata)
INSERT INTO bank_keyspace.user (username, password, account_number, fullname, balance, age) VALUES (?, ?, ?, ?, ?, ?);

-- Delete User (account number)
DELETE FROM bank_keyspace.user WHERE username = ? AND account_number = ?;

-- Update User (account number, newdata)
UPDATE bank_keyspace.user SET password = ?, fullname = ?, balance = ?, age = ? WHERE username = ? AND account_number = ?;
