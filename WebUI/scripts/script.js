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
    print("LDR File:", preview);
    print(ldrContent, preview);
    ldrContent = readLDR(ldrContent);
    print("Lego Locations:", preview);
    print(ldrContent, preview);
    ldrContent = normalizeLDR(ldrContent);
    print("Normalized Lego Locations:", preview);
    print(ldrContent, preview);
    ldrContent = sortLegos(ldrContent);
    print("Sort Legos:", preview);
    print(ldrContent, preview);
    //ldrContent = createPrintInstructions(ldrContent);
    //print("Print Instructions:", preview);
    //print(ldrContent, preview);
    print("Create .ino File:", preview);
    createINO(ldrContent, "Output", preview);
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
  return ldrContent;
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
    try {
      try {
        output.forEach(e1 => {
          e1.forEach(e2 => {
            e2.forEach(e3 => {
              let para = document.createElement('p');
              para.textContent = e3;
              printLocation.appendChild(para);
            });
          });
        })
      } catch (error) {
        output.forEach(e1 => {
          e1.forEach(e2 => {
            let para = document.createElement('p');
            para.textContent = e2;
            printLocation.appendChild(para);
          });
        })
      }
    } catch (error) {
      output.forEach(element => {
        let para = document.createElement('p');
        para.textContent = element;
        printLocation.appendChild(para);
      })
    }
  } catch (error) {
    let para = document.createElement('p');
    para.textContent = output;
    printLocation.appendChild(para);
  }
}

function createPrintInstructions(ldrContent) {
}

function sortLegos(ldrContent) {
  let splitXY = [];
  splitY(ldrContent).forEach(y => {
    splitXY.push(splitX(y));
  });
  splitXY.forEach(y => {
    y.forEach(x => {
      x.sort(function (a, b) {
        if (a.z > b.z) return -1;
        if (a.z < b.z) return 1;
        return 0;
      });
    });
  });
  return splitXY;
}

function splitY(content) {
  let yValues = []
  content.forEach(lego => {
    if (!yValues.includes(lego.y)) {
      yValues.push(lego.y);
    }
  });
  let splitArray = [];
  yValues.forEach(y => {
    let array = [];
    content.forEach(lego => {
      if (lego.y == y) {
        array.push(lego);
      }
    });
    splitArray.push(array);
  });
  return splitArray;
}

function splitX(content) {
  let xValues = []
  content.forEach(lego => {
    if (!xValues.includes(lego.x)) {
      xValues.push(lego.x);
    }
  });
  let splitArray = [];
  xValues.forEach(x => {
    let array = [];
    content.forEach(lego => {
      if (lego.x == x) {
        array.push(lego);
      }
    });
    splitArray.push(array);
  });
  return splitArray;
}

function createINO(content, outputName, printLocation) {
  const link = document.createElement("a");
  const file = new Blob([content], { type: 'text/plain' });
  link.href = URL.createObjectURL(file);
  link.download = outputName + ".ino";
  let temp = document.createElement("button");
  temp.textContent = "Download File";
  link.appendChild(temp);
  printLocation.appendChild(link);
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