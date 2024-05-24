CREATE DATABASE IF NOT EXISTS bank_database;

USE bank_database;

CREATE TABLE IF NOT EXISTS user (
    username VARCHAR(255) PRIMARY KEY,
    password VARCHAR(255),
    account_number VARCHAR(255) UNIQUE,
    fullname VARCHAR(255),
    balance DECIMAL(15,2),
    age INT,
    role ENUM('user', 'admin')
);

CREATE TABLE IF NOT EXISTS transaction_history (
    account_number VARCHAR(255),
    transaction_time TIMESTAMP,
    transaction_amount DECIMAL(15,2),
    PRIMARY KEY (account_number, transaction_time),
    FOREIGN KEY (account_number) REFERENCES user(account_number)
);

CREATE TABLE IF NOT EXISTS debug_log (
    log_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    message TEXT
);

DELIMITER //

DROP PROCEDURE IF EXISTS MakeTransfer;

CREATE PROCEDURE MakeTransfer(
    IN transfer_amount DECIMAL(15,2), 
    IN from_account_number VARCHAR(255), 
    IN to_account_number VARCHAR(255)
)
BEGIN
    DECLARE from_balance DECIMAL(15,2);

    -- Logging
    INSERT INTO debug_log (message) VALUES (CONCAT('Start transfer from ', from_account_number, ' to ', to_account_number, ' amount ', transfer_amount));

    -- Check if the from account has enough balance
    SELECT balance INTO from_balance
    FROM user
    WHERE account_number = from_account_number;

    IF from_balance IS NULL THEN
        INSERT INTO debug_log (message) VALUES ('Source account does not exist');
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Source account does not exist';
    ELSEIF from_balance < transfer_amount THEN
        INSERT INTO debug_log (message) VALUES ('Insufficient funds');
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Insufficient funds';
    ELSE
        -- Check if the to account exists
        IF (SELECT COUNT(*) FROM user WHERE account_number = to_account_number) = 0 THEN
            INSERT INTO debug_log (message) VALUES ('Destination account does not exist');
            SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Destination account does not exist';
        ELSE
            START TRANSACTION;

            INSERT INTO debug_log (message) VALUES ('Transaction started');

            -- Deduct from source account
            UPDATE user 
            SET balance = balance - transfer_amount
            WHERE account_number = from_account_number;

            -- Add to destination account
            UPDATE user 
            SET balance = balance + transfer_amount
            WHERE account_number = to_account_number;

            -- Insert transaction history for source account
            INSERT INTO transaction_history 
            (account_number, transaction_time, transaction_amount) 
            VALUES (from_account_number, NOW(), -transfer_amount);

            -- Insert transaction history for destination account
            INSERT INTO transaction_history 
            (account_number, transaction_time, transaction_amount) 
            VALUES (to_account_number, NOW(), transfer_amount);

            INSERT INTO debug_log (message) VALUES ('Transaction committed');
            
            COMMIT;
        END IF;
    END IF;

    INSERT INTO debug_log (message) VALUES ('End of transfer');
END //

DELIMITER ;

SHOW DATABASES;
USE bank_database;
SHOW TABLES;
SELECT * FROM user;
SELECT * FROM transaction_history;