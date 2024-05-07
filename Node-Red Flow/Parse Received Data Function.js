// Parse received data
var data = msg.payload.toString(); // Convert Buffer to string
var jsonData = JSON.parse(data); // Parse JSON data

// Process the received JSON data
// Here you can perform any necessary data manipulation or validation
// For example, extract values from the JSON object and construct queries
var username = jsonData.username;
var password = jsonData.password;
var accountNumber = jsonData.account_number;
var fullname = jsonData.fullname;
var balance = jsonData.balance;
var age = jsonData.age;

// Construct the Cassandra query to insert the data into the user table
var query = `
    INSERT INTO bank_keyspace.user 
    (username, password, account_number, fullname, balance, age) 
    VALUES 
    ('${username}', '${password}', '${accountNumber}', '${fullname}', ${balance}, ${age});
`;

// Create a new message object with the query as the payload
var newMsg = {
    topic: "Cassandra Query", // Topic for routing to the database node
    payload: query // Cassandra query to be executed
};

// Pass the message to the next node (Cassandra node)
return newMsg;