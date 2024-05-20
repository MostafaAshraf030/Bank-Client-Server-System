var username = msg.requestData.username;

// Your code for checking admin privileges here
var checkAdminQuery = `SELECT role FROM user WHERE username = ?`;
var checkAdminValues = [username];

context.global.mysql.query(checkAdminQuery, checkAdminValues, function(error, results, fields) {
    if (error) {
        console.error('Error checking admin privileges:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while checking admin privileges' };
        node.send(msg);
    } else {
        if (results.length === 0) {
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'User not found' };
            node.send(msg);
        } else {
            var isAdmin = results[0].role === 'admin';
            msg.payload = { isAdmin: isAdmin };
            node.send(msg);
        }
    }
});
