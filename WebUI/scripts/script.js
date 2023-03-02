const input = document.getElementById('file');
const preview = document.querySelector('.preview');

input.style.opacity = 1;
//input.addEventListener('change', readLDR);

document.querySelector('input')
  .addEventListener('change', function () {

    var fr = new FileReader();
    fr.onload = function () {
      document.querySelector('.output')
        .textContent = fr.result;
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

function returnFileSize(number) {
  if (number < 1024) {
    return `${number} bytes`;
  } else if (number >= 1024 && number < 1048576) {
    return `${(number / 1024).toFixed(1)} KB`;
  } else if (number >= 1048576) {
    return `${(number / 1048576).toFixed(1)} MB`;
  }
}