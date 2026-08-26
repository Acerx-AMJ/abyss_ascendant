document.addEventListener("DOMContentLoaded", () => 
{
   // Load all of the HTML objects.
   const container  = document.getElementById("container"),  
         xInput     = document.getElementById("sizeX"),
         yInput     = document.getElementById("sizeY"),
         selector   = document.getElementById("tileSelector"),
         generate   = document.getElementById("generate"),
         save       = document.getElementById("save"),
         result     = document.getElementById("result"),
         load       = document.getElementById("load"),
         loadButton = document.getElementById("loadButton"),
         undo       = document.getElementById("undo"),
         redo       = document.getElementById("redo"),
         clear      = document.getElementById("clear"),
         cord       = document.getElementById("cord");

   const history = [];
   const undoHistory = [];
   const tiles = [];
   let mouseDown = false;
   let tileType = "EM";
   let sizeX = 16;
   let sizeY = 16;
   let loadedLayout = "";

   // Clear all of the HTML input objects.
   xInput.value = 16;
   yInput.value = 16;
   selector.value = "EM";
   load.value = "";

   // Check if the mouse is down or up.
   document.addEventListener("mousedown", () => {mouseDown = true});
   document.addEventListener("mouseup", () => {mouseDown = false});

   // Undo an action.
   undo.addEventListener("click", () =>
   {
      if (history.length == 0) return;
      let tile = history.at(-1).at(0);
      let id = history.at(-1).at(1);
      
      undoHistory.push([tile, tile.id]);

      tile.id = id;
      tile.textContent = getFullName(id);
      history.pop();
   });

   // Redo an action.
   redo.addEventListener("click", () =>
   {
      if (undoHistory.length == 0) return;
      let tile = undoHistory.at(-1).at(0);
      let id = undoHistory.at(-1).at(1);

      history.push([tile, tile.id]);

      tile.id = id;
      tile.textContent = getFullName(id);
      undoHistory.pop();
   });

   // Clear history.
   clear.addEventListener("click", () =>
   {
      history.length = 0;
      undoHistory.length = 0;
   }),

   // Load a map from a lua file.
   loadButton.addEventListener("click", () =>
   {
      if (!load.value) return;
      loadedLayout = load.value.replace(/\s+/g, '').replace(/{|}/g, '').replace("layout=", "").split(',');
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

      for (y = 0; y < sizeY; y++)
      {
         const list = [];

         for (x = 0; x < sizeX; x++)
         {
            const obj = document.createElement("div");
   
            obj.style.left = `${x * 15}px`;
            obj.style.top = `${y * 15}px`;
            obj.classList.add("tile");

            const tType = loadedLayout[index++];
            obj.id = tType;
            obj.textContent = getFullName(tType);

            obj.addEventListener("mouseover", () =>
            {
               cord.innerText = `X: ${obj.offsetLeft / 15}, Y: ${obj.offsetTop / 15}, Type: ${obj.id}`;

               if (mouseDown)
               {
                  history.push([obj, obj.id]);
                  obj.id = tileType;
                  obj.textContent = getFullName(tileType);
               }
            });

            obj.addEventListener("click", () => 
            {
               history.push([obj, obj.id]);
               obj.id = tileType;
               obj.textContent = getFullName(tileType);
            });

            obj.addEventListener("contextmenu", (event) =>
            {
               event.preventDefault();
               tileType = obj.id;
               selector.value = tileType;
            });
   
            container.appendChild(obj);
            list.push(obj);
         }
         tiles.push(list);
      }
   });

   // Select the tile.
   selector.addEventListener("change", () => 
   {
      tileType = selector.value;
   });

   // Input the X size of the map.
   xInput.addEventListener("input", (event) => 
   {
      const value = parseInt(event.target.value);
      sizeX = value;
   });

   // Input the Y size of the map.
   yInput.addEventListener("input", (event) => 
   {
      const value = parseInt(event.target.value);
      sizeY = value;
   });

   // Generate an empty map at the given size.
   generate.addEventListener("click", () => 
   {
      tiles.forEach((list) => {list.forEach((tile) => {tile.remove()})})
      tiles.length = 0;
      history.length = 0;
      undoHistory.length = 0;

      container.style.width = `${sizeX * 15}px`;
      container.style.height = `${sizeY * 15}px`;

      for (y = 0; y < sizeY; y++)
      {
         const list = [];

         for (x = 0; x < sizeX; x++)
         {
            const obj = document.createElement("div");
   
            obj.style.left = `${x * 15}px`;
            obj.style.top = `${y * 15}px`;
            obj.classList.add("tile");
            obj.id = "EM";

            obj.addEventListener("mouseover", () =>
            {
               cord.innerText = `X: ${obj.offsetLeft / 15}, Y: ${obj.offsetTop / 15}, Type: ${obj.id}`;

               if (mouseDown)
               {
                  history.push([obj, obj.id]);
                  obj.id = tileType;
                  obj.textContent = getFullName(tileType);
               }
            });

            obj.addEventListener("click", () => 
            {
               history.push([obj, obj.id]);
               obj.id = tileType;
               obj.textContent = getFullName(tileType);
            });

            obj.addEventListener("contextmenu", (event) =>
            {
               event.preventDefault();
               tileType = obj.id;
               selector.value = tileType;
            });
   
            container.appendChild(obj);
            list.push(obj);
         }
         tiles.push(list);
      }
   });

   // Save the map as a lua script.
   save.addEventListener("click", () => 
   {
      let lua = "Result:\nlayout = {";

      for (y = 0; y < tiles.length; y++)
      {
         lua += "\n\t{";
         for (x = 0; x < tiles.at(0).length; x++)
         {
            if (getFullName(tiles.at(y).at(x).id) == "??")
               lua += "UN";
            else
               lua += tiles.at(y).at(x).id;
            
            if (x != tiles.at(0).length - 1)
               lua += ",";
         }
         lua += "},";
      }
      
      lua += "\n},"
      result.textContent = lua;
   });
});

// Get the tile symbol.
function getFullName(tile)
{
   switch (tile)
   {
   case "EM": case "OB": case "BR": return "";
   case "CO": return "●";
   case "PL": return "☻";
   case "NY": return "♠";
   case "JA": return "♞";
   case "GA": return "♦";
   case "HB": return "═";
   case "VB": return "║";
   case "GL": return "◆";
   case "SE": return "◊";
   case "LU": return "⯎";
   case "WA": return "✡";
   case "CH": return "●";
   case "BS": return "⍙";
   case "MI": return "✸";
   default: return "??";
   }
}