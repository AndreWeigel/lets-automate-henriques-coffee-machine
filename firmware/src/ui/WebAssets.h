#pragma once

#include <Arduino.h>

static const char kIndexHtml[] PROGMEM = R"HTML(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Henrique's Coffee</title>
    <link rel="stylesheet" href="/styles.css" />
  </head>
  <body>
    <main class="card">
      <h1>Henrique's Coffee</h1>
      <p id="state">State: --</p>
      <p id="message" class="muted"></p>
      <button id="start">Start Coffee</button>
    </main>
    <script src="/app.js"></script>
  </body>
</html>
)HTML";

static const char kAppJs[] PROGMEM = R"JS(
const stateEl = document.getElementById('state');
const messageEl = document.getElementById('message');
const startBtn = document.getElementById('start');

async function fetchStatus() {
  try {
    const res = await fetch('/api/status');
    if (!res.ok) return;
    const data = await res.json();
    stateEl.textContent = `State: ${data.state}`;
    messageEl.textContent = data.message || '';
  } catch (err) {
    messageEl.textContent = 'Status fetch failed';
  }
}

startBtn.addEventListener('click', async () => {
  startBtn.disabled = true;
  try {
    await fetch('/api/start', { method: 'POST' });
  } finally {
    setTimeout(() => (startBtn.disabled = false), 1000);
  }
});

fetchStatus();
setInterval(fetchStatus, 1000);
)JS";

static const char kStylesCss[] PROGMEM = R"CSS(
:root {
  color-scheme: light;
  font-family: "Fira Sans", "Segoe UI", sans-serif;
  background: radial-gradient(circle at top, #f4e7d4, #e0c9a6);
}

body {
  margin: 0;
  min-height: 100vh;
  display: grid;
  place-items: center;
}

.card {
  background: #ffffff;
  padding: 2rem 2.5rem;
  border-radius: 20px;
  box-shadow: 0 20px 40px rgba(64, 40, 14, 0.2);
  text-align: center;
}

h1 {
  margin-top: 0;
  font-size: 2rem;
  color: #6b3f1d;
}

button {
  border: none;
  border-radius: 999px;
  padding: 0.75rem 1.75rem;
  font-weight: 700;
  background: #c45a1b;
  color: white;
  cursor: pointer;
}

button:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.muted {
  color: #755b44;
}
)CSS";
