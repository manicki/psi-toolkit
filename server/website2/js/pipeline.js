$(document).ready(function(){

  $("#toolbox textarea").autosize();
  $('#input-file > input').filestyle({ });

  bindTextBookmarks();
  bindRandomExamples();
  bindChangeOutputFontSize();

  readPsisOptions();

});

function bindTextBookmarks() {
  $('#toolbox-text > .toolbox-right > .bookmark').click(function(){

    if ($('#input-text').is(':visible')) {
      switchBookmark('text', 'file');
    }
    else {
      switchBookmark('file', 'text');
    }
  });  
}

function switchBookmark(from, to) {
  if ($('#input-' + to).is(':visible')) { return; }
  
  var toggleDuration = 300;

  $('input.input-radio').attr('checked', false);

  $('#input-' + from).fadeOut(toggleDuration, function(){
    $('#input-' + from).hide();
    $('#input-' + to).fadeIn(toggleDuration);
    $('#radio-input-' + to).attr('checked', true);
  }); 
}

function bindRandomExamples() {
  var examples = [
    { 'pipe' : 'tokenize --lang pl', 
      'description' : 'Tokenizes Polish text and writes all tokens.' }, 
    { 'pipe' : 'srx-segmenter --lang en ! simple-writer --tag segment', 
      'description' : 'Splits English text into sentences.' }, 
    { 'pipe' : 'tokenize --lang pl ! morfologik ! bilexicon --lang pl --trg-lang en ! simple-writer --tag bilexicon', 
      'description' : 'Read text, tokenize, produce morphologic interpretations of eachword, generate translations for all morphological interpetations,return simplified output: filtered to show only translations.' }
  ]

  $('#random-example').click(function(e){
    e.preventDefault();
    var randomExample = Math.floor(Math.random() * examples.length)
    $('#toolbox-pipe input').val(examples[randomExample]['pipe']);
  });
}

function readPsisOptions() {
  if (psisOptions['isInputFile'] == "on") { switchBookmark('text', 'file'); }

  if (psisOptions['fileToDownload'] != "") {
    $('#output-bottom-info > .download-output').attr('href', psisOptions['fileToDownload']);
  }
  else {
    $('#output-bottom-info').css('display', 'none');
  }
}

function bindChangeOutputFontSize() {
  var minFontSize = 8;
  var maxFontSize = 20;
  var step = 2;

  $('.output-fontsize').click(function() {
    var currentFontSize = parseInt($('#output').css('font-size'));
    var currentLineHeight = parseInt($('#output').css('line-height'));

    if ($(this).attr('id') == 'increase-output-fontsize' && currentFontSize < maxFontSize) {
      $('#output').css(
        'font-size', (currentFontSize + step) + 'px'
      ).css(
        'line-height', (currentLineHeight + step) + 'px'
      );
    }

    if ($(this).attr('id') == 'decrease-output-fontsize' && currentFontSize > minFontSize) {
      $('#output').css(
        'font-size', (currentFontSize - step) + 'px'
      ).css(
        'line-height', (currentLineHeight - step) + 'px'
      );
    }
  });
}
