// Pure push_swap stack simulator. No DOM dependency.
// State shape: { a: number[], b: number[] } — index 0 is the TOP of each stack.

function clone(state) {
	return { a: state.a.slice(), b: state.b.slice() };
}

// One physical move. Tokens: sa sb ss pa pb ra rb rra rrb
function applyMove(state, move) {
	const s = clone(state);
	const { a, b } = s;
	switch (move) {
		case "sa":
			if (a.length >= 2) [a[0], a[1]] = [a[1], a[0]];
			break;
		case "sb":
			if (b.length >= 2) [b[0], b[1]] = [b[1], b[0]];
			break;
		case "ss":
			if (a.length >= 2) [a[0], a[1]] = [a[1], a[0]];
			if (b.length >= 2) [b[0], b[1]] = [b[1], b[0]];
			break;
		case "pa":
			if (b.length) a.unshift(b.shift());
			break;
		case "pb":
			if (a.length) b.unshift(a.shift());
			break;
		case "ra":
			if (a.length) a.push(a.shift());
			break;
		case "rb":
			if (b.length) b.push(b.shift());
			break;
		case "rra":
			if (a.length) a.unshift(a.pop());
			break;
		case "rrb":
			if (b.length) b.unshift(b.pop());
			break;
		default:
			throw new Error(`unknown move: ${move}`);
	}
	return s;
}

// rr/rrr are combined tokens emitted by the checker; expand into their two
// physical moves so every intermediate state can still be captured.
const EXPANSION = {
	rr: ["ra", "rb"],
	rrr: ["rra", "rrb"],
};

// Apply one op token (possibly combined) to a state. Returns the final state only.
function applyOp(state, op) {
	const moves = EXPANSION[op] || [op];
	return moves.reduce((s, m) => applyMove(s, m), state);
}

// Replay a full op list from an initial array (stack a, stack b empty).
// Returns an array of { op, move, state } snapshots, one per PHYSICAL move
// (so a combined "rr" yields two entries: move "ra" then move "rb", both
// tagged with op "rr" so the UI can show which line of push_swap's output
// they came from).
function replay(initial, ops) {
	let state = { a: initial.slice(), b: [] };
	const snapshots = [{ op: null, move: null, state: clone(state) }];
	for (const op of ops) {
		const moves = EXPANSION[op] || [op];
		for (const move of moves) {
			state = applyMove(state, move);
			snapshots.push({ op, move, state: clone(state) });
		}
	}
	return snapshots;
}

function isSorted(state) {
	if (state.b.length) return false;
	for (let i = 1; i < state.a.length; i++) {
		if (state.a[i - 1] > state.a[i]) return false;
	}
	return true;
}

if (typeof module !== "undefined" && module.exports) {
	module.exports = { applyOp, applyMove, replay, clone, isSorted };
}
