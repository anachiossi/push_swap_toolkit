(function () {
	let state = { a: [], b: [] };
	let history = []; // snapshots before each op, parallel to log entries
	let log = [];

	const elA = document.getElementById("stack-a");
	const elB = document.getElementById("stack-b");
	const elLog = document.getElementById("log");
	const elStatus = document.getElementById("status");
	const undoBtn = document.getElementById("undo-btn");

	function render() {
		renderStack(elA, state.a);
		renderStack(elB, state.b);
		elLog.innerHTML = log.map((op) => `<li>${op}</li>`).join("");
		elLog.scrollTop = elLog.scrollHeight;
		elStatus.textContent = isSorted(state) && state.a.length ? "sorted ✓" : "";
		undoBtn.disabled = history.length === 0;
	}

	function renderStack(el, arr) {
		el.innerHTML = "";
		// stack array index 0 = top; DOM is column-reverse so append top last
		// to land it visually on top — iterate bottom to top.
		for (let i = arr.length - 1; i >= 0; i--) {
			const cell = document.createElement("div");
			cell.className = "stack-cell" + (i === 0 ? " top" : "");
			cell.textContent = arr[i];
			el.appendChild(cell);
		}
	}

	function load(values) {
		state = { a: values, b: [] };
		history = [];
		log = [];
		render();
	}

	document.getElementById("load-btn").addEventListener("click", () => {
		const raw = document.getElementById("input-stack").value.trim();
		if (!raw) return;
		const values = raw.split(/\s+/).map(Number);
		if (values.some((v) => !Number.isFinite(v))) {
			alert("only integers, space separated");
			return;
		}
		if (new Set(values).size !== values.length) {
			alert("values must be unique (push_swap requirement)");
			return;
		}
		load(values);
	});

	document.getElementById("random-btn").addEventListener("click", () => {
		const n = 8;
		const pool = Array.from({ length: 99 }, (_, i) => i - 49);
		for (let i = pool.length - 1; i > 0; i--) {
			const j = Math.floor(Math.random() * (i + 1));
			[pool[i], pool[j]] = [pool[j], pool[i]];
		}
		const values = pool.slice(0, n);
		document.getElementById("input-stack").value = values.join(" ");
		load(values);
	});

	document.querySelectorAll(".op").forEach((btn) => {
		btn.addEventListener("click", () => {
			const op = btn.dataset.op;
			history.push(clone(state));
			log.push(op);
			state = applyOp(state, op);
			render();
		});
	});

	undoBtn.addEventListener("click", () => {
		if (!history.length) return;
		state = history.pop();
		log.pop();
		render();
	});

	load([5, 4, 3, 2, 1]);
})();
