/* Toggle color scheme */
function toggleMode() {
	let sun = document.querySelector('#sun');
	let moon = document.querySelector('#moon');
	let body = document.querySelector('body');
	let codeContainer = document.querySelector('.code-container');
	let text = document.querySelectorAll('h1, h2, p');
	let nav = document.querySelectorAll('h3, h4, a');
	let logoCircle = document.querySelectorAll('#el-1');
	let logoAll = document.querySelectorAll('#rect, #el-2');
	let logoSpan = document.querySelector('#logo-span');
	let up = document.querySelector('#up');
	let extLinkIcon = document.querySelector('#ext-link-path');
	let lang = document.querySelector('select');
	let arrow = document.querySelector('#arrow-stroke');
	let menu = document.querySelector('#hamburger-icon-svg');
	
	if ((moon.style.display === '') || (moon.style.display === 'flex')) {
		moon.style.display = 'none';
		sun.style.display = 'flex';
		body.style.backgroundColor = '#282A36';
		codeContainer.style.backgroundColor = '#44475a';
		text.forEach(e => e.style.color = '#FFFFFF');
		nav.forEach(e => e.style.color = '#6272A4');
		logoCircle.forEach(e => e.style.fill = '#282A36');
		logoAll.forEach(e => e.style.fill = '#6272A4');
		logoSpan.style.color = '#6272A4';
		up.style.stroke = '#282A36';
		up.style.backgroundColor = '#6272A4';
		extLinkIcon.setAttribute('stroke', '#6272A4');
		lang.style.color = '#FFFFFF';
		arrow.style.fill = '#FFFFFF';
		menu.style.fill = '#6272A4';
	} else {
		moon.style.display = 'flex';
		sun.style.display = 'none';
		body.style.backgroundColor = '#FFFFFF';
		codeContainer.style.backgroundColor = '#282A36';
		text.forEach(e => e.style.color = '#000000');
		nav.forEach(e => e.style.color = '#007DFC');
		logoCircle.forEach(e => e.style.fill = '#FFFFFF');
		logoAll.forEach(e => e.style.fill = '#007DFC');
		logoSpan.style.color = '#007DFC';
		up.style.stroke = '#FFFFFF';
		up.style.backgroundColor = '#007DFC';
		extLinkIcon.setAttribute('stroke', '#007DFC');
		lang.style.color = '#000000';
		arrow.style.fill = '#000000';
		menu.style.fill = '#007DFC';
	}
}

/* Enable select on arrow click */
function selectOnArrow() {
	document.querySelector('#pick-lang').click();
}

/* Hamburger menu handler */
function handleMenu() {
	let content = document.querySelector('.content');
	content.classList.toggle('open');
}

/* Copy code */
let copyButton = document.querySelector('.copyButton');

copyButton.addEventListener("click", () => {
	let codeSnippet = document.querySelector('code');

	let codeText = codeSnippet.innerText.trim();
	let codeWithoutPrefix = codeText.replace(/^\$\s{4}/, '');

	let dummyElement = document.createElement('textarea');
	dummyElement.value = codeWithoutPrefix;
	document.body.appendChild(dummyElement);
	dummyElement.select();
	document.execCommand('copy');
	document.body.removeChild(dummyElement);

	copyButton.classList.add('clicked');
	  
	copyButton.addEventListener('transitionend', () => {
		copyButton.classList.remove('clicked');
	}, { once: true });
});

/* Smooth scrolling */
document.addEventListener('DOMContentLoaded', function() {
    let links = document.querySelectorAll('a[href^="#"]');
    
    links.forEach(function(link) {
        link.addEventListener('click', function(e) {
            let href = this.getAttribute('href');

            if (href !== '#') {
                e.preventDefault();

                let target = document.querySelector(href);
                if (target) {
                    target.scrollIntoView({
                        behavior: 'smooth'
                    });
                }
            }
        });
    });
});
