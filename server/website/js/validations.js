$(document).ready(function()
{
    var preloaderTimestamp = 3000;
    var messages = [];

    function clearMessages() {
        messages = [];
        $("#message").empty();
    };

    function addMessage(text) {
        messages.push("<li>" + text + "</li>");
    };

    function showMessage() {
        if (messages.length) {
            var mess = $("<ul>").html(messages.join(''));
            $("#message").html("<p>Some errors occured:</p>").append(mess);
            return true;
        }
        return false;
    };

    function showPreloader() {
        setTimeout( function() {
            $("#message").addClass("preloader");
	    }, preloaderTimestamp);
    };

    $("form").submit(function() {
        clearMessages();

        if (!$("#input-pipe").val()) {
            addMessage("Pipe specification cannot be empty.");
        }

        if ($("#radio-input-text").attr("checked") == 'checked') {
            if (!$("#input-text textarea").val()) {
                addMessage("Write some text to process.");
            }
        }
        else {
            if (!$("#input-file input").val()) {
                addMessage("Select file with text to process.");
            }
        }

        if (showMessage()) {
            return false;
        }
        else {
           showPreloader();
        }
        $(this).submit();
    });


});

