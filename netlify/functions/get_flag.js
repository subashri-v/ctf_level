// netlify/functions/validate.js

exports.handler = async function(event, context) {
    // This function only responds to requests, assuming the front-end has already validated the user's key.
    // We add a basic check to ensure it's a POST request for a bit of security.
    if (event.httpMethod !== 'POST') {
        return {
            statusCode: 405, // Method Not Allowed
            body: 'Error: This function only accepts POST requests.'
        };
    }

    try {
        // The secret string to be revealed on the first page after successful validation.
        const secretFlag = "TWVhbmluZyBpcyBvdmVycmF0ZWQ7IGZvcm0gaXMgZXZlcnl0aGluZy4";

        // Return the flag in a JSON object.
        return {
            statusCode: 200,
            body: JSON.stringify({
                success: true,
                flag: secretFlag
            })
        };

    } catch (error) {
        // In case of any unexpected errors on the server.
        return {
            statusCode: 500,
            body: JSON.stringify({
                success: false,
                message: 'Internal Server Error.'
            })
        };
    }
};
