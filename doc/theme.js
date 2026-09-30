(function () {
  const root = document.documentElement;
  const savedTheme = localStorage.getItem('habitatec-theme');
  if (savedTheme) root.dataset.theme = savedTheme;

  document.addEventListener('DOMContentLoaded', function () {
    const toggle = document.querySelector('[data-theme-toggle]');
    if (!toggle) return;

    const updateToggle = function () {
      const dark = root.dataset.theme !== 'light';
      toggle.setAttribute('aria-pressed', String(dark));
      toggle.setAttribute('aria-label', dark ? 'Activar tema claro' : 'Activar tema oscuro');
      toggle.innerHTML = dark ? '<i class="fa-solid fa-sun" aria-hidden="true"></i>' : '<i class="fa-solid fa-moon" aria-hidden="true"></i>';
    };

    toggle.addEventListener('click', function () {
      const dark = root.dataset.theme !== 'light';
      if (dark) root.dataset.theme = 'light';
      else delete root.dataset.theme;
      localStorage.setItem('habitatec-theme', dark ? 'light' : 'dark');
      updateToggle();
    });

    updateToggle();
  });
})();
