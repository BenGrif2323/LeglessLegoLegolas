const input = document.getElementById('file');
const preview = document.querySelector('.preview');

input.style.opacity = 1;

document.querySelector('input').addEventListener('change', function () {

  let fr = new FileReader();
  fr.onload = function () {
    let ldrContent = "";
    ldrContent = fr.result;
    ldrContent = ldrContent.split(/\r?\n/);
    document.querySelector('.output').textContent = '';
    print(ldrContent, preview);
    ldrContent = readLDR(ldrContent);

  }

  fr.readAsText(this.files[0]);
})

function readLDR(ldrContent) {
  const legos = [];
  ldrContent.forEach(element => {
    
  })
}

function print(output, printLocation) {
  output.forEach(element => {
    let para = document.createElement('p');
    para.textContent = element;
    printLocation.appendChild(para);
  })
}

class Lego {
  constructor(i, j, k) {
    this.x = i;
    this.y = j;
    this.z = k;
  }
  toString() {
    return '(' + x + ', ' + y + ', ' + z + ')';
  }
}