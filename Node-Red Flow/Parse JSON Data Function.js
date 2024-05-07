// Parse the JSON data retrieved from the file
try {
    var jsonData = JSON.parse(msg.payload);

    // Extract data from the JSON object
    var username = jsonData.username;
    var password = jsonData.password;
    var accountNumber = jsonData.account_number;
    var fullname = jsonData.fullname;
    var balance = jsonData.balance;
    var age = jsonData.age;

    // Construct the Cassandra insertion query
    var query = `
        INSERT INTO bank_keyspace.user 
        (username, password, account_number, fullname, balance, age) 
        VALUES 
        (?, ?, ?, ?, ?, ?)
    `;

    // Parameters to be passed with the query
    var params = [username, password, accountNumber, fullname, balance, age];

    // Create a new message object with the query and parameters
    var newMsg = {
        topic: "Cassandra Query", // Topic for routing to the database node
        payload: {
            query: query, // Cassandra query to be executed
            params: params // Parameters for the query
        }
    };

    // Pass the message to the next node (Cassandra node)
    return newMsg;
} catch (error) {
    // Handle parsing errors
    console.error("Error parsing JSON:", error.message);
    // Optionally, you can return an error message or handle the error in a different way
}