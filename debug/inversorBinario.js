const fs = require('fs');
const path = require('path');

// Função para converter um binário em decimal
function binToDec(bin) {
  return parseInt(bin, 2);
}

// Função para processar o arquivo e identificar as instruções
function processInstructions() {
  const inputFilePath = path.join(__dirname, 'codigoBinario.txt');
  const outputFilePath = path.join(__dirname, 'codigoInverso.txt');

  // Lê o arquivo codigoBinario.txt linha a linha
  const binaryLines = fs.readFileSync(inputFilePath, 'utf-8').trim().split('\n');

  let output = [];

  binaryLines.forEach((binary) => {
    let instruction = '';
    let opcode = binary.slice(0, 6);

    if (opcode === '000000') {
      // Tipo R (aritmética)
      let RD = binToDec(binary.slice(6, 11));
      let rt = binToDec(binary.slice(11, 16));
      let RS = binToDec(binary.slice(16, 21));
      let funct = binary.slice(26, 32);

      let functName = '';
      switch (funct) {
        case '000000': functName = 'soma'; break;
        case '000001': functName = 'sub'; break;
        case '000010': functName = 'mult'; break;
        case '000011': functName = 'div'; break;
        case '000100': functName = 'and'; break;
        case '000101': functName = 'or'; break;
        case '000110': functName = 'invert'; break;
        default: functName = 'desconhecida';
      }

      instruction = `instrução: ${functName}, RD: ${RD}, RT: ${rt}, RS: ${RS}, IMEDIATO: não usado, ENDEREÇO: não usado`;

    } else if (['000001', '000010', '000100', '001101', '000101', '000110', '000111', '001000', '001001', '001010', '001011', '001100'].includes(opcode)) {
      // Tipo I
      let RD = binToDec(binary.slice(6, 11));
      let rt = binToDec(binary.slice(11, 16));
      let immediate = binToDec(binary.slice(16, 32));

      let instructionName = '';
      switch (opcode) {
        case '000001': instructionName = 'addi'; break;
        case '000010': instructionName = 'subi'; break;
        case '000100': instructionName = 'jr'; break;
        case '001101': instructionName = 'jal'; break;
        case '000101': instructionName = 'beq'; break;
        case '000110': instructionName = 'bnq'; break;
        case '000111': instructionName = 'blt'; break;
        case '001000': instructionName = 'bgt'; break;
        case '001001': instructionName = 'ble'; break;
        case '001010': instructionName = 'bge'; break;
        case '001011': instructionName = 'lw'; break;
        case '001100': instructionName = 'sw'; break;
        default: instructionName = 'desconhecida';
      }

      instruction = `instrução: ${instructionName}, RD: ${RD}, RT: ${rt}, RS: não usado, IMEDIATO: ${immediate}, ENDEREÇO: não usado`;

    } else if (['000011', '010001', '001111', '001110'].includes(opcode)) {
      // Tipo J
      let address = binToDec(binary.slice(6, 32));

      let instructionName = '';
      switch (opcode) {
        case '000011': instructionName = 'jump'; break;
        case '010001': instructionName = 'halt'; break;
        case '001111': instructionName = 'in'; break;
        case '001110': instructionName = 'out'; break;
        default: instructionName = 'desconhecida';
      }

      instruction = `instrução: ${instructionName}, RD: não usado, RT: não usado, RS: não usado, IMEDIATO: não usado, ENDEREÇO: ${address}`;
    }

    // Adiciona a instrução processada à lista de saídas
    output.push(instruction);
  });

  // Escreve o resultado no arquivo codigoInverso.txt
  fs.writeFileSync(outputFilePath, output.join('\n'), 'utf-8');
  console.log('Processamento concluído e resultado salvo em codigoInverso.txt');
}

// Executa o processamento
processInstructions();
