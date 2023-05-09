const input = document.getElementById('file');
const preview = document.querySelector('.preview');

input.style.opacity = 1;

var ldrContent = '';
document.getElementById('file').addEventListener('change', function () {
  ldrContent = '';
  let fr = new FileReader();
  fr.onload = function () {
    ldrContent = fr.result;
    ldrContent = ldrContent.split(/\r?\n/);
    document.querySelector('.preview').innerHTML = '';
    print('LDR File:', preview);
    print(ldrContent, preview);
  }

  fr.readAsText(this.files[0]);
})

document.getElementById('submit').addEventListener('click', function () {
  if (ldrContent != '') {
    document.querySelector('.preview').innerHTML = '';
    document.getElementById('download').innerHTML = '';
    print('LDR File:', preview);
    print(ldrContent, preview);
    let ldrInfo = ldrContent;
    ldrInfo = readLDR(ldrInfo);
    //print('Lego Locations:', preview);
    //print(ldrInfo, preview);
    ldrInfo = normalizeLDR(ldrInfo);
    //print('Normalized Lego Locations:', preview);
    //print(ldrInfo, preview);
    ldrInfo = sortLegos(ldrInfo);
    //print('Sort Legos:', preview);
    //print(ldrInfo, preview);
    ldrInfo = createPrintInstructions(ldrInfo);
   // print('Print Instructions:', preview);
    //print(ldrInfo, preview);
    //print('Create .ino File:', preview);
    createINO(ldrInfo, 'Output', document.getElementById('download'));
  }
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
  let pinDisableMotors = 46;
  try {
    pinDisableMotors = document.getElementById('disableMotors').value;
  } catch (error) {
    console.log(error);
  }
  let pinDirX = 45;
  try {
    pinDirX = document.getElementById('dirX').value;
  } catch (error) {
    console.log(error);
  }
  let pinStepX = 40;
  try {
    pinStepX = document.getElementById('stepX').value;
  } catch (error) {
    console.log(error);
  }
  let stepSizeX = 1;
  try {
    let temp = parseFloat(document.getElementById('stepSizeX').value);
    if (temp >= 0) {
      stepSizeX = temp;
    }
    else {
      throw "Error with stepSizeX, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let pinDirY = 21;
  try {
    pinDirY = document.getElementById('dirY').value;
  } catch (error) {
    console.log(error);
  }
  let pinStepY = 47;
  try {
    pinStepY = document.getElementById('stepY').value;
  } catch (error) {
    console.log(error);
  }
  let stepSizeY = 1;
  try {
    let temp = parseFloat(document.getElementById('stepSizeY').value);
    if (temp >= 0) {
      stepSizeY = temp;
    }
    else {
      throw "Error with stepSizeY, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let pinDirZ = 14;
  try {
    pinDirZ = document.getElementById('dirZ').value;
  } catch (error) {
    console.log(error);
  }
  let pinStepZ = 39;
  try {
    pinStepZ = document.getElementById('stepZ').value;
  } catch (error) {
    console.log(error);
  }
  let stepSizeZ = 1;
  try {
    let temp = parseFloat(document.getElementById('stepSizeZ').value);
    if (temp >= 0) {
      stepSizeZ = temp;
    }
    else {
      throw "Error with stepSizeZ, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let stepDelay = 40;
  try {
    let temp = parseInt(document.getElementById('stepDelay').value);
    if (temp >= 0) {
      stepDelay = temp;
    }
    else {
      throw "Error with stepDelay, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let xBound = 500;
  try {
    let temp = parseFloat(document.getElementById('xBound').value);
    if (temp >= 0) {
      xBound = temp;
    }
    else {
      throw "Error with xBound, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let yBound = 500;
  try {
    let temp = parseFloat(document.getElementById('yBound').value);
    if (temp >= 0) {
      yBound = temp;
    }
    else {
      throw "Error with yBound, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let zBound = 500;
  try {
    let temp = parseFloat(document.getElementById('zBound').value);
    if (temp >= 0) {
      zBound = temp;
    }
    else {
      throw "Error with zBound, using default value";
    }
  } catch (error) {
    console.log(error);
  }
  let headPosX = 0;
  let headPosY = 0;
  let headPosZ = 0;
  let currentY = 0;

  function beginINO() {
    return '#define PIN_STEP_X ' + pinStepX + '\n' + //default 40
      '#define PIN_STEP_Y ' + pinStepY + '\n' + //default 47
      '#define PIN_STEP_Z ' + pinStepZ + '\n' + //default 39
      //'#define PIN_STEP_4 ' + pinStep4 + '\n' + //default 37
      '#define PIN_DIR_X ' + pinDirX + '\n' + //default 45
      '#define PIN_DIR_Y ' + pinDirY + '\n' + //default 21
      '#define PIN_DIR_Z ' + pinDirZ + '\n' + //default 14
      //'#define PIN_DIR_4 ' + pinDir4 + '\n' + //default 35
      '#define PIN_DISABLE_MOTORS ' + pinDisableMotors + '\n\n' + //default 46
      'int stepDelay = ' + stepDelay + ';\n\n' + //default 40
      'void move(int x, int absX, int y, int absY, int z, int absZ, int max) {\n' +
      '  digitalWrite(PIN_DIR_X, x >= 0);\n' +
      '  digitalWrite(PIN_DIR_Y, y >= 0);\n' +
      '  digitalWrite(PIN_DIR_Z, z >= 0);\n' +
      '  for (int i = 0; i < max; i++) {\n' +
      '    if (i < absX) {\n' +
      '      digitalWrite(PIN_STEP_X, HIGH);\n' +
      '    }\n' +
      '    if (i < absY) {\n' +
      '      digitalWrite(PIN_STEP_Y, HIGH);\n' +
      '    }\n' +
      '    if (i < absZ) {\n' +
      '      digitalWrite(PIN_STEP_Z, HIGH);\n' +
      '    }\n' +
      '    delayMicroseconds(stepDelay);\n' +
      '    digitalWrite(PIN_STEP_X, LOW);\n' +
      '    digitalWrite(PIN_STEP_Y, LOW);\n' +
      '    digitalWrite(PIN_STEP_Z, LOW);\n' +
      '    delayMicroseconds(stepDelay);\n' +
      '  }\n' +
      '}\n\n' +
      'void initializeHead() {\n' +
      '  //Code to Initialize Print Head\n' +
      '}\n\n' +
      'void pickUpLego() {\n' +
      '  //Code to Pick Up a Lego\n' +
      '}\n\n' +
      'void placeLego() {\n' +
      '  //Code to Place a Lego\n' +
      '}\n\n' +
      'void setup() {\n' +
      '  //put your setup code here, to run once:\n' +
      '  pinMode(PIN_DISABLE_MOTORS, OUTPUT);\n' +
      '  pinMode(PIN_STEP_X, OUTPUT);\n' +
      '  pinMode(PIN_STEP_Y, OUTPUT);\n' +
      '  pinMode(PIN_STEP_Z, OUTPUT);\n' +
      '  //pinMode(PIN_STEP_4, OUTPUT);\n' +
      '  pinMode(PIN_DIR_X, OUTPUT);\n' +
      '  pinMode(PIN_DIR_Y, OUTPUT);\n' +
      '  pinMode(PIN_DIR_Z, OUTPUT);\n' +
      '  //pinMode(PIN_DIR_4, OUTPUT);\n' +
      '  delayMicroseconds(1000000);\n\n';
  }

  function initializePrintHead() {
    headPosX = 0;
    headPosY = 0;
    headPosZ = 0;
    return '  initializeHead();\n\n';
  }

  function moveTo(x, y, z) {
    tempX = (x - headPosX) / stepSizeX;
    tempY = (y - headPosY) / stepSizeY;
    tempZ = (z - headPosZ) / stepSizeZ;
    tempMax = Math.max(Math.max(Math.abs(tempX), Math.abs(tempY)), Math.abs(tempZ));
    headPosX = x;
    headPosY = y;
    headPosZ = z;
    return '  move(' + tempX + ', ' + Math.abs(tempX) + ', '
      + tempY + ', ' + Math.abs(tempY) + ', ' + tempZ
      + ', ' + Math.abs(tempZ) + ', ' + tempMax + ');\n\n';;
  }

  function legoReload() {
    return moveTo(0, currentY, 0) + moveTo(0, 0, 0) +
      '  pickUpLego();\n\n' + moveTo(0, currentY, 0);
  }

  function legoPlace(lego) {
    currentY = lego.y;
    return moveTo(lego.x, lego.y, lego.z) + '  placeLego();\n\n';
  }

  function endINO() {
    return moveTo(0, yBound, 0) + '}\n\n' +
      'void loop() {\n' +
      '}';
  }

  //Create File Text
  let output = beginINO();
  output += initializePrintHead();
  //Print Instructions
  try {
    ldrContent.forEach(ySplit => {
      ySplit.forEach(xSplit => {
        xSplit.forEach(lego => {
          //console.log(lego);
          if (lego.x <= xBound && lego.y <= yBound && lego.z <= zBound) {
            output += legoReload();
            output += legoPlace(lego);
          }
          else {
            console.log('Lego is out of bound: ' + lego);
            throw 'Lego is out of bounds: ' + lego;
          }
        });
      });
    })
  }
  catch (error) {
    print(error, preview);
    return error;
  }
  output += endINO();
  return output;
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
  try {
    let output = '';
    content.forEach(e => {
      output += e + '\n';
    })
    const link = document.createElement('a');
    const file = new Blob([output], { type: 'text/plain' });
    link.href = URL.createObjectURL(file);
    link.download = outputName + '.ino';
    let temp = document.createElement('button');
    temp.textContent = 'Download File';
    link.appendChild(temp);
    printLocation.appendChild(link);
  } catch (error) {
    const link = document.createElement('a');
    const file = new Blob([content], { type: 'text/plain' });
    link.href = URL.createObjectURL(file);
    link.download = outputName + '.ino';
    let temp = document.createElement('button');
    temp.textContent = 'Download File';
    link.appendChild(temp);
    printLocation.appendChild(link);
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