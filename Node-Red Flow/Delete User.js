var username = msg.requestData.username;

// Your code for deleting a user here
var deleteUserQuery = `DELETE FROM user WHERE username = ?`;
var deleteUserValues = [username];

context.global.mysql.query(deleteUserQuery, deleteUserValues, function(error, results, fields) {
    if (error) {
        console.error('Error deleting user:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while deleting the user' };
        node.send(msg);
    } else {
        // Check if any rows were affected
        if (results.affectedRows === 0) {
            msg.statusCode = 404; // Not Found
            msg.payload = { error: 'User not found' };
            node.send(msg);
        } else {
            msg.payload = { success: true };
            node.send(msg);
        }
    }
});
