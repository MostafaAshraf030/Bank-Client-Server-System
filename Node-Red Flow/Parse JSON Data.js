// Assuming jsonData is an array containing the JSON data from the file
var jsonData = msg.payload; // Assuming the JSON data is passed as payload

// Initialize an empty array to store individual insertion messages
var insertionMessages = [];

// Loop through the JSON array
jsonData.forEach(function(item) {
    // Extract relevant information from each JSON object for the 'user' table
    var userInsertionQuery = `
        INSERT INTO user 
        (username, password, account_number, fullname, balance, age, role) 
        VALUES 
        ('${item.username}', '${item.password}', '${item.account_number}', '${item.fullname}', ${item.balance}, ${item.age}, '${item.role}')
    `;

    // Create a message object with the insertion query for the 'user' table
    var userInsertionMessage = {
        topic: "MySQL Insertion", // Topic for routing to the MySQL node
        payload: userInsertionQuery // MySQL query to be executed
    };

    // Push the message object into the array
    insertionMessages.push(userInsertionMessage);

    // Construct insertion message for the 'transaction_history' table
    var transactionHistoryInsertionQuery = `
        INSERT INTO transaction_history 
        (account_number, transaction_time, transaction_amount) 
        VALUES 
        ('${item.account_number}', NOW(), ${item.balance})
    `;

    // Create a message object with the insertion query for the 'transaction_history' table
    var transactionHistoryInsertionMessage = {
        topic: "MySQL Insertion", // Topic for routing to the MySQL node
        payload: transactionHistoryInsertionQuery // MySQL query to be executed
    };

    // Push the message object into the array
    insertionMessages.push(transactionHistoryInsertionMessage);
});

// Now insertionMessages array contains individual insertion messages for each data entry
// You can use this array in your Node-RED flow to pass each insertion message to the MySQL node
return insertionMessages;