(function(){
  var root=document.documentElement, btn=document.getElementById('theme-btn');
  try{var saved=localStorage.getItem('theme'); if(saved) root.setAttribute('data-theme',saved);}catch(e){}
  btn.addEventListener('click',function(){
    var cur=root.getAttribute('data-theme') || (matchMedia('(prefers-color-scheme: dark)').matches?'dark':'light');
    var next=cur==='dark'?'light':'dark'; root.setAttribute('data-theme',next);
    try{localStorage.setItem('theme',next);}catch(e){}
  });
  var copy=document.getElementById('copy-btn'), email=document.getElementById('email');
  if(copy&&email)copy.addEventListener('click',function(){
    var t=email.textContent;
    var done=function(){copy.textContent='Copied';setTimeout(function(){copy.textContent='Copy email'},1600)};
    var fallback=function(){var r=document.createRange();r.selectNodeContents(email);var s=getSelection();s.removeAllRanges();s.addRange(r);copy.textContent='Selected, press Ctrl+C'};
    if(navigator.clipboard){navigator.clipboard.writeText(t).then(done,fallback)}else{fallback()}
  });
})();
