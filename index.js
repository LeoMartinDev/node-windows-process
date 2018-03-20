const addon = require('./build/Release/addon.node');

module.exports = {
  getProcessesByName,
  getProcessesByNameAsync,
  getCurrentProcess,
  getCurrentProcessAsync,
  getProcessById,
  getProcessByIdAsync,
  terminateProcessById,
  setToForegroundAsync,
};

function getProcessesByName(name) {
  return addon.getProcessesByName(name);
}

function getProcessesByNameAsync(name) {
  return new Promise((resolve, reject) => {
    addon.getProcessesByNameAsync(name, function (error, result) {
      if (error) return reject(error);
      return resolve(result);
    });
  });
}

function getCurrentProcess() {
  return addon.getCurrentProcess();
}

function getCurrentProcessAsync() {
  return new Promise((resolve, reject) => {
    addon.getCurrentProcessAsync(function (error, result) {
      if (error) return reject(error);
      return resolve(result);
    });
  });
}

function getProcessById(id) {
  return addon.getProcessById(id);
}

function getProcessByIdAsync(id) {
  return new Promise((resolve, reject) => {
    addon.getProcessByIdAsync(id, function (error, result) {
      if (error) return reject(error);
      return resolve(result);
    });
  });
}

function terminateProcessById(id) {
  return addon.terminateProcessById(id);
}

function setToForegroundAsync(hwmd) {
  return new Promise((resolve, reject) => {
    addon.setToForegroundAsync(hwmd, function (error, result) {
      if (error) return reject(error);
      return resolve(result);
    });
  });
}