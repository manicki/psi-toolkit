$(document).ready(function(){
  bindBibtexToggle();
});

function bindBibtexToggle() {
  $('a.bibtex-toggler').click(function(){
    $(this).next().toggle('fast');
  });
}
