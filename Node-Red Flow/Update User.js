var username = msg.requestData.username;
var password = msg.requestData.password;
var fullname = msg.requestData.fullname;
var age = msg.requestData.age;

// Your code for updating a user here
var updateUserQuery = `UPDATE user SET password = ?, fullname = ?, age = ? WHERE username = ?`;
var updateUserValues = [password, fullname, age, username];

context.global.mysql.query(updateUserQuery, updateUserValues, function(error, results, fields) {
    if (error) {
        console.error('Error updating user:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while updating the user' };
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
