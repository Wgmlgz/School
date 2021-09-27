let offset_x = 0
let offset_y = 0

let down_x = 0
let down_y = 0

let is_down = false
let can = false
let start = true
var zoom = 1

export function btnDown(e) {
  console.log(can);
  if (can) {
    is_down = true;
  }
  down_x = e.screenX
  down_y = e.screenY
  
}
export function btnUp(e) {
  is_down = false;
  let st = document.getElementById('tree').style
  offset_x = parseInt(st.left)
  offset_y = parseInt(st.top)
}
export function dragTree(e) {
  let x = e.screenX
  let y = e.screenY

  if (is_down || start) {
    let st = document.getElementById('tree').style
    if (start) {
      st.left = 0 + 'px'
      st.top = 0 + 'px'
    } else {
      st.left = (x - down_x) * (1 / zoom) + offset_x + 'px'
      st.top = (y - down_y) * (1 / zoom)+ offset_y + 'px'
    }

    start = false
  }
}
export function mouseEnter() {
  console.log('enter');
  can = true
}
export function mouseLeft() {
  can = false
  console.log('over');
}
export function setZoom(val) {
  zoom = val
}