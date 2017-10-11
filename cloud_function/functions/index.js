const functions = require('firebase-functions');

const admin = require('firebase-admin');
admin.initializeApp(functions.config().firebase);

const ApiAiApp = require('actions-on-google').ApiAiApp;

exports.receiveAssistantRequests = functions.https.onRequest((request, response) => {

    const app = new ApiAiApp({request: request, response: response});

    function handlerRequest(app) {

        const device = app.getArgument('devices');
        const status = app.getArgument('status');

        admin.database().ref(`/automation/${device}/value`).set(status)
            .then(snapshot => {
                app.ask(`Ok, switching ${device} ${status}. Do you want to control anything else?`);
                response.send(200);
            });

    }

    app.handleRequest(handlerRequest);
});
