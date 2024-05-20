var username = msg.requestData.username;
var password = msg.requestData.password;

var query = "SELECT role FROM user WHERE username = ? AND password = ?";

// Execute the query
context.global.mysql.query(query, [username, password], function(error, results, fields) {
    if (error) {
        console.error('Error checking credentials:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while checking credentials' };
        node.send(msg);
    } else {
        // Check if the credentials are valid
        if (results.length > 0) {
            var role = results[0].role;
            msg.payload = { isAuthenticated: true, role: role };
            node.send(msg);
        } else {
            console.error('Invalid username or password');
            msg.statusCode = 401; // Unauthorized
            msg.payload = { error: 'Invalid username or password' };
            node.send(msg);
        }
    }
});
