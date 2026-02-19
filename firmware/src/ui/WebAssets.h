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
      <div class="state-cycle" id="state-cycle" aria-label="Coffee cycle states"></div>
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
const cycleEl = document.getElementById('state-cycle');

const defaultStates = [
  'IDLE',
  'PRESS_BUTTON',
  'WAIT_READY',
  'ACTUATE_LEVER',
  'BREWING',
  'RETURN_LEVER',
  'DONE'
];

let stateListKey = '';

function buildStateCycle(states) {
  cycleEl.style.setProperty('--count', states.length);
  cycleEl.innerHTML = '';
  states.forEach((state, index) => {
    const node = document.createElement('div');
    node.className = 'state-node';
    node.dataset.state = state;
    node.style.setProperty('--i', index);

    const label = document.createElement('span');
    label.className = 'state-label';
    label.textContent = state;

    node.appendChild(label);
    cycleEl.appendChild(node);
  });
}

function ensureStateCycle(states) {
  const key = JSON.stringify(states);
  if (key !== stateListKey) {
    stateListKey = key;
    buildStateCycle(states);
  }
}

function updateActiveState(activeState, isError) {
  const nodes = cycleEl.querySelectorAll('.state-node');
  nodes.forEach((node) => {
    const isActive = node.dataset.state === activeState;
    node.classList.toggle('is-active', isActive);
    node.classList.toggle('is-error', isActive && isError);
  });
}

async function fetchStatus() {
  try {
    const res = await fetch('/api/status');
    if (!res.ok) return;
    const data = await res.json();
    const states = Array.isArray(data.states) && data.states.length ? data.states : defaultStates;
    ensureStateCycle(states);
    updateActiveState(data.state, data.error);
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
  --coffee-brown: #6b3f1d;
  --coffee-accent: #c45a1b;
  --coffee-light: #fff7ec;
  --coffee-muted: #755b44;
  --coffee-error: #b3261e;
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
  color: var(--coffee-brown);
}

.state-cycle {
  --size: min(70vw, 320px);
  --radius: calc(var(--size) / 2 - 44px);
  width: var(--size);
  height: var(--size);
  margin: 1.5rem auto 1rem;
  position: relative;
  display: grid;
  place-items: center;
}

.state-cycle::before {
  content: "";
  position: absolute;
  inset: 24px;
  border-radius: 50%;
  border: 0;
}

.state-node {
  position: absolute;
  top: 50%;
  left: 50%;
  width: 90px;
  transform:
    translate(-50%, -50%)
    rotate(calc(360deg / var(--count) * var(--i)))
    translateY(calc(-1 * var(--radius)))
    rotate(calc(-360deg / var(--count) * var(--i)));
  transform-origin: center;
  text-align: center;
  color: var(--coffee-muted);
  font-size: 0.7rem;
  letter-spacing: 0.02em;
}

.state-node::before {
  content: "";
  display: block;
  width: 38px;
  height: 38px;
  margin: 0 auto 0.4rem;
  border-radius: 50%;
  border: 2px solid var(--coffee-accent);
  background: var(--coffee-light);
  box-shadow: 0 6px 12px rgba(107, 63, 29, 0.12);
}

.state-node.is-active::before {
  border-color: var(--coffee-brown);
}

.state-node.is-error::before {
  border-color: var(--coffee-error);
  background: #fde8e7;
}

.state-node.is-active::after {
  content: "";
  position: absolute;
  top: -4px;
  left: 50%;
  width: 46px;
  height: 46px;
  border-radius: 50%;
  border: 2px solid rgba(107, 63, 29, 0.5);
  transform: translateX(-50%);
  animation: pulse 1.8s ease-out infinite;
}

.state-node.is-error::after {
  border-color: rgba(179, 38, 30, 0.6);
}

@keyframes pulse {
  0% {
    opacity: 0.6;
    transform: translateX(-50%) scale(0.9);
  }
  70% {
    opacity: 0;
    transform: translateX(-50%) scale(1.4);
  }
  100% {
    opacity: 0;
    transform: translateX(-50%) scale(1.4);
  }
}

button {
  border: none;
  border-radius: 999px;
  padding: 0.75rem 1.75rem;
  font-weight: 700;
  background: var(--coffee-accent);
  color: white;
  cursor: pointer;
}

button:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.muted {
  color: var(--coffee-muted);
}
)CSS";
