-- Create Keyspace
CREATE KEYSPACE IF NOT EXISTS bank_keyspace
WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};

-- Create User Table with Role Field
CREATE TABLE IF NOT EXISTS bank_keyspace.user (
    username TEXT PRIMARY KEY,
    password TEXT,
    account_number TEXT,
    fullname TEXT,
    balance INT,
    age INT,
    role TEXT  -- Added role field
);

-- Create Transaction History Table
CREATE TABLE IF NOT EXISTS bank_keyspace.transaction_history (
    account_number TEXT,
    transaction_time TIMESTAMP,
    transaction_amount INT,
    PRIMARY KEY (account_number, transaction_time)
);

-- Make Transfer (from account number, to account number, transfer amount)
-- Update balances for both from and to account numbers and insert records into the transaction_history table
-- Parameters: transfer_amount, from_account_number, to_account_number
BEGIN BATCH
    UPDATE bank_keyspace.user 
    SET balance = balance - ?  -- Deduct transfer amount from source balance
    WHERE account_number = ?;

    UPDATE bank_keyspace.user 
    SET balance = balance + ?  -- Add transfer amount to destination balance
    WHERE account_number = ?;

    INSERT INTO bank_keyspace.transaction_history 
    (account_number, transaction_time, transaction_amount) 
    VALUES (?, toTimestamp(now()), ?);  -- Use current timestamp for transaction time

    INSERT INTO bank_keyspace.transaction_history 
    (account_number, transaction_time, transaction_amount) 
    VALUES (?, toTimestamp(now()), ?);  -- Use current timestamp for transaction time
APPLY BATCH;

-- View Bank Database
SELECT * FROM bank_keyspace.user;

-- Create New User (userdata)
-- Parameters: username, password, account_number, fullname, balance, age, role
INSERT INTO bank_keyspace.user (username, password, account_number, fullname, balance, age, role) 
VALUES (?, ?, ?, ?, ?, ?, ?);

-- Delete User (account number)
-- Parameters: username, account_number
DELETE FROM bank_keyspace.user WHERE username = ? AND account_number = ?;

-- Update User (account number, newdata)
-- Parameters: new_password, new_fullname, new_balance, new_age, username, account_number
UPDATE bank_keyspace.user 
SET password = ?, fullname = ?, balance = ?, age = ? 
WHERE username = ? AND account_number = ?;