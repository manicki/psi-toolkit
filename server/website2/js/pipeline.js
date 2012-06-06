$(document).ready(function(){

  $("#toolbox textarea").autosize();
  $('#input-file > input').filestyle({ });

  bindTextBookmarks();
  bindRandomExamples();
  bindChangeOutputSize();

  readPsisOptions();

});

function bindTextBookmarks() {
  $('#toolbox-text .bookmarks > a').click(function(e){
  e.preventDefault();

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

  $('#bookmark-' + from).fadeOut(toggleDuration, function(){
    $('#bookmark-' + from).hide();
    $('#bookmark-' + to).fadeIn(toggleDuration);
  });

}

function bindRandomExamples() {
  var pipeInput = $('#toolbox-pipe input');

  var examples = [
    { 'pipe' : 'tokenize --lang pl',
      'description' : 'Tokenizes Polish text and writes all tokens.' },
    { 'pipe' : 'srx-segmenter --lang en ! simple-writer --tag segment',
      'description' : 'Splits English text into sentences.' },
    { 'pipe' : 'tokenize --lang pl ! morfologik ! bilexicon --lang pl --trg-lang en ! simple-writer --tag bilexicon',
      'description' : 'Read text, tokenize, produce morphologic interpretations of eachword, generate translations for all morphological interpetations, return simplified output: filtered to show only translations.' }
  ]

  pipeInput.attr('rel', 'tipsy');
  $('#toolbox-pipe input').tipsy({
      trigger : 'manual',
      gravity : 's'
  });

  $('#random-example').click(function(e){
    e.preventDefault();
    var randomExample = Math.floor(Math.random() * examples.length);

    pipeInput.val(examples[randomExample]['pipe']);
    pipeInput.attr('title', examples[randomExample]['description']);
    pipeInput.tipsy('show');

    e.stopPropagation();
    return false;
  });

  $('body').click(function(){
    pipeInput.tipsy('hide');
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

function bindChangeOutputSize() {

  if (isOutputType('text')) {
    var query = '#output';
    changeOutputFontSize(query, 8, 20, 2);
  }
  else if (isOutputType('svg')) {
    var query = '#output > svg';
    var width = parseInt($(query).attr('width'));

    changeOutputSVGSize(query,
      width - Math.round(0.75*width), width + Math.round(0.25*width), Math.round(0.1*width)
    );
  }
  else if (isOutputType('image')) {
    var query = '#output > a > img';
    var width = parseInt($(query).width());

    changeOutputImageSize(query,
      width - Math.round(0.75*width), width + Math.round(0.25*width), Math.round(0.1*width)
    );
  }

}

function changeOutputFontSize(query, min, max, step) {
  $('.output-fontsize').click(function() {

    var currentFontSize = parseInt($(query).css('font-size'));
    var currentLineHeight = parseInt($(query).css('line-height'));

    if ($(this).attr('id') == 'increase-output-fontsize' && currentFontSize < max) {
      $(query).css('font-size', (currentFontSize + step) + 'px')
        .css('line-height', (currentLineHeight + step) + 'px');
    }

    if ($(this).attr('id') == 'decrease-output-fontsize' && currentFontSize > min) {
      $(query).css('font-size', (currentFontSize - step) + 'px')
        .css('line-height', (currentLineHeight - step) + 'px');
    }
  });
}

function changeOutputSVGSize(query, min, max, step) {
  $('.output-fontsize').click(function() {
    var currentSVGWidth = parseInt($(query).attr('width'));

    if ($(this).attr('id') == 'increase-output-fontsize' && currentSVGWidth < max) {
      $(query).attr('width', (currentSVGWidth + step) + 'pt');
    }
    if ($(this).attr('id') == 'decrease-output-fontsize' && currentSVGWidth > min) {
      $(query).attr('width', (currentSVGWidth - step) + 'pt');
    }
  });
}

function changeOutputImageSize(query, min, max, step) {
  $('.output-fontsize').click(function() {
    var currentImageWidth = parseInt($(query).width());

    if ($(this).attr('id') == 'increase-output-fontsize' && currentImageWidth < max) {
      $(query).css('width', (currentImageWidth + step) + 'px');
    }
    if ($(this).attr('id') == 'decrease-output-fontsize' && currentImageWidth > min) {
      $(query).css('width', (currentImageWidth - step) + 'px');
    }
  });
}

function isOutputType(type) {
  switch(type) {
    case 'text':
      return $('#output > pre').length ? true : false;
      break;
    case 'svg':
      return $('#output > svg').length ? true : false;
      break;
    case 'image':
      return $('#output > a > img').length ? true : false;
      break;
  }
};
