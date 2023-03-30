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
    //print("LDR File:", preview);
    //print(ldrContent, preview);
    ldrContent = readLDR(ldrContent);
    //print("Lego Locations:", preview);
    //print(ldrContent, preview);
    normalizeLDR(ldrContent);
    print("Normalized Lego Locations:", preview);
    print(ldrContent, preview);
  }

  fr.readAsText(this.files[0]);
})

function normalizeLDR(ldrContent) {
  let xNorm = Number.MAX_VALUE;
  let yNorm = Number.MAX_VALUE;
  let zNorm = Number.MAX_VALUE;
  ldrContent.forEach(lego => {
    if (parseInt(lego.x) < xNorm) {
      xNorm = lego.x;
    }
    if (parseInt(lego.y) < yNorm) {
      yNorm = lego.y;
    }
    if (parseInt(lego.z) < zNorm) {
      zNorm = lego.z;
    }
  })
  ldrContent.forEach(lego => {
    lego.normalize(xNorm, yNorm, zNorm);
  })
}

function readLDR(ldrContent) {
  const legos = [];
  ldrContent.forEach(line => {
    if (line.startsWith('1')) {
      let temp = line.split(' ');
      let x = temp[2];
      let y = temp[3];
      let z = temp[4];
      legos.push(new Lego(x, y, z));
    }

  })
  return legos;
}

function print(output, printLocation) {
  try {
    output.forEach(element => {
      let para = document.createElement('p');
      para.textContent = element;
      printLocation.appendChild(para);
    })
  } catch (error) {
    let para = document.createElement('p');
    para.textContent = output;
    printLocation.appendChild(para);
  }

}

class Lego {
  constructor(i, j, k) {
    this.x = i;
    this.y = j;
    this.z = k;
  }

  toString() {
    return '(' + this.x + ', ' + this.y + ', ' + this.z + ')';
  }

  normalize(i, j, k) {
    this.x = this.x - i;
    this.y = this.y - j;
    this.z = this.z - k;
  }
}