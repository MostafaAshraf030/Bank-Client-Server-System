-- Create Keyspace
CREATE KEYSPACE IF NOT EXISTS bank_keyspace
WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};

<<<<<<< HEAD
-- Create User Table with Role Field
=======
-- Create User Table
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
CREATE TABLE IF NOT EXISTS bank_keyspace.user (
    username TEXT PRIMARY KEY,
    password TEXT,
    account_number TEXT,
    fullname TEXT,
    balance INT,
<<<<<<< HEAD
    age INT,
    role TEXT  -- Added role field
);

-- Create Transaction History Table
=======
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
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
CREATE TABLE IF NOT EXISTS bank_keyspace.transaction_history (
    account_number TEXT,
    transaction_time TIMESTAMP,
    transaction_amount INT,
    PRIMARY KEY (account_number, transaction_time)
);

<<<<<<< HEAD
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
=======
-- Make Transaction (account number, transaction amount)
-- This would involve updating the balance in the user table and inserting a record into the transaction_history table

-- Make Transfer (from account number, to account number, transfer amount)
-- This would involve updating balances for both from and to account numbers and inserting records into the transaction_history table
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74

-- View Bank Database
SELECT * FROM bank_keyspace.user;

-- Create New User (userdata)
<<<<<<< HEAD
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
=======
INSERT INTO bank_keyspace.user (username, password, account_number, fullname, balance, age) VALUES (?, ?, ?, ?, ?, ?);

-- Delete User (account number)
DELETE FROM bank_keyspace.user WHERE username = ? AND account_number = ?;

-- Update User (account number, newdata)
UPDATE bank_keyspace.user SET password = ?, fullname = ?, balance = ?, age = ? WHERE username = ? AND account_number = ?;
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
