var query = "SELECT account_number FROM user WHERE username = ?";

// Execute the query
context.global.mysql.query(query, [username], function(error, results, fields) {
    if (error) {
        console.error('Error retrieving account number:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while retrieving account number' };
        node.send(msg);
    } else {
        // Check if the username exists
        if (results.length > 0) {
            var accountNumber = results[0].account_number;
            msg.payload = { accountNumber: accountNumber };
            node.send(msg);
        } else {
            console.error('Username not found');
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'Username not found' };
            node.send(msg);
        }
    }
});