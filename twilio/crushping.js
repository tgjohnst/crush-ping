// This was originally sourced from Alex Swan's lovenotes project at https://particle.hackster.io/boldbigflank/love-notes-37be49 
const request = require('request')

exports.handler = function(context, event, callback) {
    // Send the message to the Particle Electron's 'message' function
    request
        .post('https://api.particle.io/v1/devices/' + context.PARTICLE_DEVICE_ID + '/message')
        .form({
            access_token: context.PARTICLE_ACCESS_TOKEN,
            args: event.Body
        })
        .on('response', function(response) {
            // Tell the sender if it worked
            let responseText = response.statusCode == 200 ? "Success!" : "Failure!"
            let twiml = new Twilio.twiml.MessagingResponse();
            twiml.message(responseText);
            callback(null, twiml);
        })
};