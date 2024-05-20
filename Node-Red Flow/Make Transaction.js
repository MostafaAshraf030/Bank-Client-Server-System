var accountNumber = msg.requestData.accountNumber;
var amount = msg.requestData.amount;

// Your code for making transaction here
var query = "UPDATE user SET balance = balance - ? WHERE account_number = ?";

// Execute the query to deduct the amount from the account balance
context.global.mysql.query(query, [amount, accountNumber], function(error, results, fields) {
    if (error) {
        console.error('Error deducting amount from account balance:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while making the transaction' };
        node.send(msg);
    } else {
        if (results.affectedRows === 0) {
            console.error('No user found with account number:', accountNumber);
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'Account number not found' };
            node.send(msg);
        } else {
            // Insert transaction history
            var insertQuery = "INSERT INTO transaction_history (account_number, transaction_time, transaction_amount) VALUES (?, NOW(), ?)";
            context.global.mysql.query(insertQuery, [accountNumber, -amount], function(error, results, fields) {
                if (error) {
                    console.error('Error inserting transaction history:', error);
                    msg.statusCode = 500; // Internal Server Error
                    msg.payload = { error: 'An error occurred while making the transaction' };
                    node.send(msg);
                } else {
                    msg.payload = { success: true };
                    node.send(msg);
                }
            });
        }
    }
});
