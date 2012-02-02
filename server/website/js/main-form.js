var preloaderTimestamp = 2000;
var messages = [];

$(document).ready(function()
{
    $('textarea.resizable:not(.processed)').TextAreaResizer();

    $("#bookmarks li a").click(function(e) {
        e.preventDefault();
        switchBookmark($(this).attr("class"));
        return false;
    });

    if (psisOptions['isInputFile'] == "on") {
        switchBookmark("input-file");
    }

    if (psisOptions['fileToDownload'] != "") {
        var infoDownload = "Click here to download the output file";
        $("#download").append("<a href=\"" + psisOptions['fileToDownload'] + "\">" + infoDownload + "</a>");
    }

   $("form").submit(function() {
        clearMessages();

        if (validate() > 0) {
            showMessage();
            return false;
        }
        else {
            //hideOldOutput();
            showPreloader();
        }
    });

});

function switchBookmark(anchorClass) {
    $("#bookmarks .active").removeClass("active");
    $('#bookmarks a.' + anchorClass).parent().addClass("active");

    // handle div content
    $("#inputs div.hidable:visible").hide();
    $("#inputs div#" + anchorClass).show();

    // handle radio buttons for server
    $("#inputs input.input-radio").attr("checked", false);
    $("#inputs input#radio-" + anchorClass).attr("checked", true);
};

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
    return errors;
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

function hideOldOutput() {
    $('#output').hide();
    $('#download').hide();
}
