var preloaderTimestamp = 2000;
var messages = [];

$(document).ready(function()
{

   $("form").submit(function() {
        clearMessages();

        if (validate() > 0) {
            showMessage();
            return false;
        }
        else {
            showPreloader();
        }
    });

});

function clearMessages() {
    messages = [];
    $("#message").empty();
};

function validate() {
    var errors = 0;

    if (!$("#input-pipe").val()) {
        addMessage("Pipe specification cannot be empty.");
        errors += 1;
    }

    if ($("#radio-input-text").attr("checked") == 'checked') {
        if (!$("#input-text textarea").val()) {
            addMessage("Write some text to process.");
            errors += 1;
        }
    }
    else {
        if (!$("#input-file input").val()) {
            addMessage("Select file with text to process.");
            errors += 1;
        }
    }
}

function addMessage(text) {
    messages.push("<li>" + text + "</li>");
};

function showMessage() {
    if (messages.length) {
        var msg = $("<ul>").html(messages.join(''));
        $("#message").html("<p>Some errors occured:</p>").append(msg);
        return true;
    }
    return false;
};

function showPreloader() {
    setTimeout( function() {
        $("#message").addClass("preloader");
    }, preloaderTimestamp);
};
