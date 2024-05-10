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
    var role = jsonData.role; // Include role field

    // Check the user's role before proceeding
    if (role === 'user' || role === 'admin') {
        // Construct the Cassandra insertion query
        var query = `
            INSERT INTO bank_keyspace.user 
            (username, password, account_number, fullname, balance, age, role) 
            VALUES 
            (?, ?, ?, ?, ?, ?, ?)
        `;

        // Parameters to be passed with the query
        var params = [username, password, accountNumber, fullname, balance, age, role];

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
    } else {
        // Handle invalid user role
        console.error("Invalid user role:", role);
        // Optionally, return an error message or handle the error in a different way
    }
} catch (error) {
    // Handle parsing errors
    console.error("Error parsing JSON:", error.message);
    // Optionally, you can return an error message or handle the error in a different way
}
