var accountNumber = msg.requestData.accountNumber;

// Your code for getting account balance by number here
var query = "SELECT balance FROM user WHERE account_number = ?";

// Execute the query
context.global.mysql.query(query, [accountNumber], function(error, results, fields) {
    if (error) {
        console.error('Error getting account balance:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while getting account balance' };
        node.send(msg);
    } else {
        // Check if the account balance is found
        if (results.length > 0) {
            var balance = results[0].balance;
            msg.payload = { balance: balance };
            node.send(msg);
        } else {
            console.error('Account number not found:', accountNumber);
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'Account number not found' };
            node.send(msg);
        }
    }
});
