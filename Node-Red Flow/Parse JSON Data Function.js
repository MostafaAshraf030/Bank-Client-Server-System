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
<<<<<<< HEAD
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
=======

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
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
} catch (error) {
    // Handle parsing errors
    console.error("Error parsing JSON:", error.message);
    // Optionally, you can return an error message or handle the error in a different way
<<<<<<< HEAD
}
=======
}
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
