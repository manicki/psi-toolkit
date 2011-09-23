
$().ready(function()
{
    $('textarea.resizable:not(.processed)').TextAreaResizer();

    $("ul#input-bookmarks li a").click(function(e)
    {
        e.preventDefault();

        $("ul#input-bookmarks .active").removeClass("active");
        $(this).parent().addClass("active");

        var class_name = $(this).attr("class");

        // handle div content
        $("#inputs div.hidable:visible").hide();
        $("#inputs div#" + class_name).show();

        // handle radio buttons for server
        $("#inputs input.input-radio").attr("checked", false);
        $("#inputs input#radio-" + class_name).attr("checked", true);

        return false;
    });

//    $("#pipe-form").submit(function()
//    {
//        if ($("#inputs input#radio-button"))
//        return true;
//    });

});

