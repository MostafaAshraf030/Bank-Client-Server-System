// Your code for getting the bank database here
var getBankDatabaseQuery = "SELECT * FROM user";

context.global.mysql.query(getBankDatabaseQuery, function(error, results, fields) {
    if (error) {
        console.error('Error retrieving bank database:', error);
        msg.statusCode = 500; // Internal Server Error
        msg.payload = { error: 'An error occurred while retrieving bank database' };
        node.send(msg);
    } else {
        msg.payload = { bankDatabase: results };
        node.send(msg);
    }
});
