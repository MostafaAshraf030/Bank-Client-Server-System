var username = msg.requestData.username;
var password = msg.requestData.password;
var fullname = msg.requestData.fullname;
var age = msg.requestData.age;

// Your code for creating a new user here
var createUserQuery = `INSERT INTO user (username, password, fullname, age) VALUES (?, ?, ?, ?)`;
var createUserValues = [username, password, fullname, age];

context.global.mysql.query(createUserQuery, createUserValues, function(error, results, fields) {
    if (error) {
        console.error('Error creating new user:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while creating a new user' };
        node.send(msg);
    } else {
        msg.payload = { success: true };
        node.send(msg);
    }
});
