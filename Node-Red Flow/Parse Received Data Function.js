// Parse received data
var data = msg.payload.toString(); // Convert Buffer to string
var jsonData = JSON.parse(data); // Parse JSON data

// Extract values from the JSON object
var username = jsonData.username;
var password = jsonData.password;
var accountNumber = jsonData.account_number;
var fullname = jsonData.fullname;
var balance = jsonData.balance;
var age = jsonData.age;
var role = jsonData.role; // Include role field

// Check the user's role before proceeding
if (role === 'user' || role === 'admin') {
    // Construct the Cassandra query to insert the data into the user table
    var query = `
        INSERT INTO bank_keyspace.user 
        (username, password, account_number, fullname, balance, age, role) 
        VALUES 
        ('${username}', '${password}', '${accountNumber}', '${fullname}', ${balance}, ${age}, '${role}');
    `;

    // Create a new message object with the query as the payload
    var newMsg = {
        topic: "Cassandra Query", // Topic for routing to the database node
        payload: query // Cassandra query to be executed
    };

    // Pass the message to the next node (Cassandra node)
    return newMsg;
} else {
    // Handle invalid user role
    console.error("Invalid user role:", role);
    // Optionally, return an error message or handle the error in a different way
}
