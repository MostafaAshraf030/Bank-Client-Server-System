var parts = msg.req.url.split("/");
var endpoint = parts[3];
var requestData = msg.payload;

// Input validation
switch (endpoint) {
    case "checkCredentials":
        if (!requestData.username || !requestData.password) {
            return { error: "Username and password are required" };
        }
        break;
    case "getAccountNumberByUsername":
        if (!requestData.username) {
            return { error: "Username is required" };
        }
        break;
    case "getAccountBalanceByNumber":
        if (!requestData.accountNumber) {
            return { error: "Account number is required" };
        }
        break;
    case "getTransactionHistoryByNumber":
        if (!requestData.accountNumber) {
            return { error: "Account number is required" };
        }
        break;
    case "makeTransaction":
        if (!requestData.accountNumber || !requestData.amount) {
            return { error: "Account number and amount are required" };
        }
        break;
    case "makeTransfer":
        if (!requestData.from_account_number || !requestData.to_account_number || !requestData.transfer_amount) {
            return { error: "From account number, to account number, and transfer amount are required" };
        }
        break;
    case "getBankDatabase":
        // No specific validation required for this endpoint
        break;
    case "createNewUser":
        if (!requestData.username || !requestData.password || !requestData.fullname || !requestData.age) {
            return { error: "Username, password, fullname, and age are required for creating a new user" };
        }
        break;
    case "deleteUser":
        if (!requestData.username) {
            return { error: "Username is required" };
        }
        break;
    case "updateUser":
        if (!requestData.username || !requestData.password || !requestData.fullname || !requestData.age) {
            return { error: "Username, password, fullname, and age are required for updating a user" };
        }
        break;
    case "checkAdminPrivileges":
        if (!requestData.username) {
            return { error: "Username is required" };
        }
        break;
    default:
        return { error: "Invalid endpoint" };
}

// If all validation passes, return the endpoint and requestData
return { endpoint: endpoint, requestData: requestData };
