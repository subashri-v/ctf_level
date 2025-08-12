exports.handler = async function(event, context) {
  const secretFlag = "TWVhbmluZyBpcyBvdmVycmF0ZWQ7IGZvcm0gaXMgZXZlcnl0aGluZy4";
  return {
    statusCode: 200,
    body: JSON.stringify({ flag: secretFlag }),
  };
};
