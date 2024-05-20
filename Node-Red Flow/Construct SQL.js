var item = msg.payload;

// Construct SQL INSERT statement for the 'user' table
var userSql = `
    INSERT INTO user (username, password, account_number, fullname, balance, age, role) 
    VALUES ('${item.username}', '${item.password}', '${item.account_number}', '${item.fullname}', ${item.balance}, ${item.age}, '${item.role}')
`;

// Construct SQL INSERT statement for the 'transaction_history' table
var transactionHistorySql = `
    INSERT INTO transaction_history (account_number, transaction_time, transaction_amount) 
    VALUES ('${item.account_number}', NOW(), ${item.balance})
`;

// Construct a single string containing both SQL queries
var sqlQueries = userSql + ';' + transactionHistorySql;

// Pass the concatenated SQL queries to the next node (MySQL node)
return { payload: sqlQueries };
