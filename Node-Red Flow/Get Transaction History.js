var accountNumber = msg.requestData.accountNumber;
var count = msg.requestData.count;

// Your code for getting transaction history by number here
var query = "SELECT transaction_time, transaction_amount FROM transaction_history WHERE account_number = ? ORDER BY transaction_time DESC LIMIT ?";

// Execute the query
context.global.mysql.query(query, [accountNumber, count], function(error, results, fields) {
    if (error) {
        console.error('Error getting transaction history:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while getting transaction history' };
        node.send(msg);
    } else {
        // Check if any transactions are found
        if (results.length > 0) {
            var history = results.map(function(transaction) {
                return `${transaction.transaction_time}: $${transaction.transaction_amount}`;
            });
            msg.payload = { history: history.join('\n') };
            node.send(msg);
        } else {
            console.error('No transaction history found for account number:', accountNumber);
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'No transaction history found' };
            node.send(msg);
        }
    }
});
