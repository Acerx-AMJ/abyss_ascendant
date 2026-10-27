document.addEventListener('DOMContentLoaded', () => {
   const container  = document.getElementById('container'),  
         xInput     = document.getElementById('sizeX'),
         yInput     = document.getElementById('sizeY'),
         selector   = document.getElementById('tileSelector'),
         generate   = document.getElementById('generate'),
         save       = document.getElementById('save'),
         result     = document.getElementById('result'),
         load       = document.getElementById('load'),
         loadButton = document.getElementById('loadButton'),
         undo       = document.getElementById('undo'),
         redo       = document.getElementById('redo'),
         cord       = document.getElementById('cord');

   const history = [];
   const undoHistory = [];
   const tiles = [];

   let leftDown = false;
   let rightDown = false;

   let tileType = 'EM';
   let sizeX = 16;
   let sizeY = 16;
   let loadedLayout = '';

   xInput.value = 16;
   yInput.value = 16;
   selector.value = 'EM';
   load.value = '';

   // prevent context menu from popping up
   document.addEventListener('contextmenu', (event) => {
      event.preventDefault();
   });

   // Check if the mouse is down or up.
   document.addEventListener('mousedown', (event) => {
      const left = (event.button == 0);
      const right = (event.button == 2);

      if (left) {
         leftDown = true;
      }
      else if (right) {
         rightDown = true;
      }
   });

   document.addEventListener('mouseup', (event) => {
      const left = (event.button == 0);
      const right = (event.button == 2);

      if (left) {
         leftDown = false;
      }
      else if (right) {
         rightDown = false;
      }
   });

   // Handle menu bar UI elements
   undo.addEventListener('click', () => {
      if (history.length == 0) {
         return;
      }
      const lastTile = history[-1][0];
      const lastId = history[-1][1];
      undoHistory.push([lastTile, lastTile.id]);

      lastTile.id = lastId;
      lastTile.textContent = getFullName(lastId);
      history.pop();
   });

   redo.addEventListener('click', () => {
      if (undoHistory.length == 0) {
         return;
      }
      const lastTile = undoHistory[-1][0];
      const lastId = undoHistory[-1][1];
      history.push([lastTile, lastTile.id]);

      tile.id = lastId;
      tile.textContent = getFullName(lastId);
      undoHistory.pop();
   });

   selector.addEventListener('change', () => {
      tileType = selector.value;
   });

   xInput.addEventListener('input', (event) => {
      const value = parseInt(event.target.value);
      sizeX = value;
   });

   yInput.addEventListener('input', (event) => {
      const value = parseInt(event.target.value);
      sizeY = value;
   });

   save.addEventListener('click', () => {
      const height = tiles.length;
      const width = tiles[0].length;
      let lua = 'Result:\nlayout = {';

      for (y = 0; y < height; y++) {
         lua += '\n\t{';
         for (x = 0; x < width; x++) {
            lua += tiles[y][x].id;
            if (x < width - 1) {
               lua += ',';
            }
         }
         lua += '},';
      }
      
      lua += '\n},'
      result.textContent = lua;
   });

   // Handle editor
   function createTile(x, y, tileId) {
      const obj = document.createElement('div');
      obj.style.left = `${x * 15}px`;
      obj.style.top = `${y * 15}px`;
      obj.classList.add('tile');
      obj.id = tileId;
      obj.textContent = getFullName(tileId);

      obj.addEventListener('mouseover', () => {
         cord.innerText = `X: ${x + 1}, Y: ${y + 1}, Type: ${obj.id}`;

         if (leftDown || rightDown) {
            const newId = (rightDown ? 'EM' : tileType);
            history.push([obj, obj.id]);
            obj.id = newId;
            obj.textContent = getFullName(newId);
         }
      });

      obj.addEventListener('mousedown', (event) => {
         const left = (event.button == 0);
         const middle = (event.button == 1);
         const right = (event.button == 2);

         if (left || right) {
            const newId = (right ? 'EM' : tileType);
            history.push([obj, obj.id]);
            obj.id = newId;
            obj.textContent = getFullName(newId);
         }
         else if (middle) {
            event.preventDefault();
            tileType = obj.id;
            selector.value = tileType;
         }
      });
      return obj;
   }

   loadButton.addEventListener('click', () => {
      if (!load.value) {
         return;
      }

      loadedLayout = load.value.replace(/\s+/g, '').replace(/{|}/g, '').replace('layout=', '').split(',');
      history.length = 0;
      undoHistory.length = 0;

      sizeY = load.value.split('}').length - 2;
      sizeX = Math.round((load.value.split(',').length - 1) / sizeY);

      xInput.value = sizeX;
      yInput.value = sizeY;

      tiles.forEach((list) => {list.forEach((tile) => {tile.remove()})})
      tiles.length = 0;

      container.style.width = `${sizeX * 15}px`;
      container.style.height = `${sizeY * 15}px`;

      let index = 0;
      for (y = 0; y < sizeY; y++) {
         const list = [];
         for (x = 0; x < sizeX; x++) {
            const obj = createTile(x, y, loadedLayout[index++]);
            container.appendChild(obj);
            list.push(obj);
         }
         tiles.push(list);
      }
   });

   generate.addEventListener('click', () => {
      tiles.forEach((list) => {list.forEach((tile) => {tile.remove()})})
      tiles.length = 0;
      history.length = 0;
      undoHistory.length = 0;

      container.style.width = `${sizeX * 15}px`;
      container.style.height = `${sizeY * 15}px`;

      for (y = 0; y < sizeY; y++) {
         const list = [];
         for (x = 0; x < sizeX; x++) {
            const obj = createTile(x, y, 'EM');
            container.appendChild(obj);
            list.push(obj);
         }
         tiles.push(list);
      }
   });
});

function getFullName(tile) {
   switch (tile) {
   case 'EM': case 'OB': case 'BR': return '';
   case 'CO': return '●';
   case 'PL': return '☻';
   case 'NY': return '♠';
   case 'JA': return '♞';
   case 'GA': return '♦';
   case 'HB': return '═';
   case 'VB': return '║';
   case 'GL': return '◆';
   case 'SE': return '◊';
   case 'LU': return '⯎';
   case 'WA': return '✡';
   case 'CH': return '●';
   case 'BS': return '⍙';
   case 'HU': return '@';
   case 'MI': return '✸';
   default: return '??';
   }
}
