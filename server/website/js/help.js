$(document).ready(function(){
  var menu = $('#help-menu');
  var origOffsetY = menu.offset().top;

  function onScroll(e) {
    window.scrollY >= origOffsetY ? menu.addClass('sticky-menu') : menu.removeClass('sticky-menu');
  }

  document.addEventListener('scroll', onScroll);
})
