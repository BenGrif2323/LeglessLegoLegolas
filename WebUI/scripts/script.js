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

  }

  fr.readAsText(this.files[0]);
})

function readLDR() {
  while (preview.firstChild) {
    preview.removeChild(preview.firstChild);
  }

  const curFiles = input.files;
  if (curFiles.length === 0) {
    const para = document.createElement('p');
    para.textContent = 'No files currently selected';
    preview.appendChild(para);
  }
  else if (curFiles.length > 1) {
    const para = document.createElement('p');
    para.textContent = 'More than one file selected';
    preview.appendChild(para);
  }
  else {
    const list = document.createElement('ul');
    preview.appendChild(list);
    const file = curFiles.item(0);
    let fileReader = new FileReader();
    fileReader.readAsText(file);
    const listItem = document.createElement('li');
    const para = document.createElement('li');
    para.textContent = fileReader.result;
    list.appendChild(para);
    list.appendChild(listItem);

  }
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