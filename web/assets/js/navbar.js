var toggle = document.getElementsByClassName('navbar-toggler')[0];
var collapse = document.getElementsByClassName('navbar-collapse')[0];

function toggleMenu() {
    collapse.classList.toggle('collapse');
    collapse.classList.toggle('in');
}

function closeMenusOnResize() {
    if (document.body.clientWidth >= 768) {
        collapse.classList.add('collapse');
        collapse.classList.remove('in');
    }
}

// Event listeners
window.addEventListener('resize', closeMenusOnResize, false);
toggle.addEventListener('click', toggleMenu, false);