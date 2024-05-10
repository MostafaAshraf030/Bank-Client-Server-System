// Parse the incoming message payload
var requestData = msg.payload;

// Declare variables
var accountNumber, query, newMsg;

// Check the type of request
if (requestData.type === 'insert') {
    // Handle insert request
    var username = requestData.data.username;
    var password = requestData.data.password;
    accountNumber = requestData.data.account_number;
    var fullname = requestData.data.fullname;
    var balance = requestData.data.balance;
    var age = requestData.data.age;
<<<<<<< HEAD
    var role = requestData.data.role; // Include role field

    // Check the user's role before proceeding
    if (role === 'user' || role === 'admin') {
        // Construct the Cassandra insertion query
        query = `
            INSERT INTO bank_keyspace.user 
            (username, password, account_number, fullname, balance, age, role) 
            VALUES 
            (?, ?, ?, ?, ?, ?, ?)
        `;

        // Parameters to be passed with the query
        var params = [username, password, accountNumber, fullname, balance, age, role];

        // Create a new message object with the query and parameters
        newMsg = {
            topic: "Cassandra Query", // Topic for routing to the database node
            payload: {
                query: query, // Cassandra query to be executed
                params: params // Parameters for the query
            }
        };
    } else {
        // Handle invalid user role
        console.error("Invalid user role:", role);
        // Optionally, return an error message or handle the error in a different way
    }
=======

    // Construct the Cassandra insertion query
    query = `
        INSERT INTO bank_keyspace.user 
        (username, password, account_number, fullname, balance, age) 
        VALUES 
        (?, ?, ?, ?, ?, ?)
    `;

    // Parameters to be passed with the query
    var params = [username, password, accountNumber, fullname, balance, age];

    // Create a new message object with the query and parameters
    newMsg = {
        topic: "Cassandra Query", // Topic for routing to the database node
        payload: {
            query: query, // Cassandra query to be executed
            params: params // Parameters for the query
        }
    };
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
} else if (requestData.type === 'update_balance') {
    // Handle update balance request
    accountNumber = requestData.data.account_number;
    var newBalance = requestData.data.new_balance;
<<<<<<< HEAD
    role = requestData.data.role; // Include role field

    // Check the user's role before proceeding
    if (role === 'admin') {
        // Construct Cassandra query to update balance for the account
        query = `
            UPDATE bank_keyspace.user 
            SET balance = ${newBalance}
            WHERE account_number = '${accountNumber}'
        `;

        // Create a new message object with the query
        newMsg = {
            topic: "Cassandra Query", // Topic for routing to the database node
            payload: query // Cassandra query to be executed
        };
    } else {
        // Handle unauthorized access
        console.error("Unauthorized access: Insufficient privileges for updating balance");
        // Optionally, return an error message or handle the error in a different way
    }
=======

    // Construct Cassandra query to update balance for the account
    query = `
        UPDATE bank_keyspace.user 
        SET balance = ${newBalance}
        WHERE account_number = '${accountNumber}'
    `;

    // Create a new message object with the query
    newMsg = {
        topic: "Cassandra Query", // Topic for routing to the database node
        payload: query // Cassandra query to be executed
    };
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
} else {
    // Handle unknown request type
    node.warn("Unknown request type: " + requestData.type);
    newMsg = null;
}

// Pass the message to the next node (Cassandra node)
<<<<<<< HEAD
return newMsg;
=======
return newMsg;
>>>>>>> 2d72460567a706e0fcec3850d1755ccaf2744a74
