$(document).ready(function()
{
    $('textarea.resizable:not(.processed)').TextAreaResizer();

    $("ul#input-bookmarks li a").click(function(e)
    {
        e.preventDefault();

        $("ul#input-bookmarks .active").removeClass("active");
        $(this).parent().addClass("active");

        switchBookmark($(this).attr("class"));

        return false;
    });

    function switchBookmark(class_name) {
        // handle div content
        $("#inputs div.hidable:visible").hide();
        $("#inputs div#" + class_name).show();

        // handle radio buttons for server
        $("#inputs input.input-radio").attr("checked", false);
        $("#inputs input#radio-" + class_name).attr("checked", true);
    }

    var options = $("div#psis-hidden-options div");

    if (options.attr("input_file") == "on") {
        $("ul#input-bookmarks .active").removeClass("active");
        $("ul#input-bookmarks a.input-file").parent().addClass("active");

        switchBookmark("input-file");
    }

    var downloadFilePath = options.attr("file_to_download");
    if (downloadFilePath != "") {
        var infoDownload = "Click here to download the output file";
        $("#download").append("<a href=\"" + downloadFilePath + "\">" + infoDownload + "</a>");
    }

});

