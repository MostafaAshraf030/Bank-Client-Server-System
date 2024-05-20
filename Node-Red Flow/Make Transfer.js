var from_account_number = msg.requestData.from_account_number;
var to_account_number = msg.requestData.to_account_number;
var transfer_amount = msg.requestData.transfer_amount;

// Check if the source account exists and has enough balance
var checkBalanceQuery = "SELECT balance FROM user WHERE account_number = ?";
context.global.mysql.query(checkBalanceQuery, [from_account_number], function(error, results, fields) {
    if (error) {
        console.error('Error checking balance for source account:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while making the transfer' };
        node.send(msg);
    } else {
        if (results.length === 0) {
            console.error('Source account does not exist:', from_account_number);
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'Source account does not exist' };
            node.send(msg);
        } else {
            var from_balance = results[0].balance;
            if (from_balance < transfer_amount) {
                console.error('Insufficient funds in source account:', from_account_number);
                msg.statusCode = 400; // Bad Request
                msg.payload = { error: 'Insufficient funds' };
                node.send(msg);
            } else {
                // Deduct from source account
                var updateFromAccountQuery = "UPDATE user SET balance = balance - ? WHERE account_number = ?";
                context.global.mysql.query(updateFromAccountQuery, [transfer_amount, from_account_number], function(error, results, fields) {
                    if (error) {
                        console.error('Error deducting amount from source account:', error);
                        msg.statusCode = 500; // Internal Server Error
                        msg.payload = { error: 'An error occurred while making the transfer' };
                        node.send(msg);
                    } else {
                        // Add to destination account
                        var updateToAccountQuery = "UPDATE user SET balance = balance + ? WHERE account_number = ?";
                        context.global.mysql.query(updateToAccountQuery, [transfer_amount, to_account_number], function(error, results, fields) {
                            if (error) {
                                console.error('Error adding amount to destination account:', error);
                                msg.statusCode = 500; // Internal Server Error
                                msg.payload = { error: 'An error occurred while making the transfer' };
                                node.send(msg);
                            } else {
                                // Insert transaction history for source account
                                var insertTransactionQuery = "INSERT INTO transaction_history (account_number, transaction_time, transaction_amount) VALUES (?, NOW(), ?)";
                                context.global.mysql.query(insertTransactionQuery, [from_account_number, -transfer_amount], function(error, results, fields) {
                                    if (error) {
                                        console.error('Error inserting transaction history for source account:', error);
                                        msg.statusCode = 500; // Internal Server Error
                                        msg.payload = { error: 'An error occurred while making the transfer' };
                                        node.send(msg);
                                    } else {
                                        // Insert transaction history for destination account
                                        context.global.mysql.query(insertTransactionQuery, [to_account_number, transfer_amount], function(error, results, fields) {
                                            if (error) {
                                                console.error('Error inserting transaction history for destination account:', error);
                                                msg.statusCode = 500; // Internal Server Error
                                                msg.payload = { error: 'An error occurred while making the transfer' };
                                                node.send(msg);
                                            } else {
                                                msg.payload = { success: true };
                                                node.send(msg);
                                            }
                                        });
                                    }
                                });
                            }
                        });
                    }
                });
            }
        }
    }
});
