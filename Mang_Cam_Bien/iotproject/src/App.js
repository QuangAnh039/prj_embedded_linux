import React from 'react';
import DashboardPage from './Page/DashboardPage';
import Profile from './Page/Profile';
import DataSensor from './Page/DataSensor';
import ActionHistory from './Page/ActionHistory';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Sidebar from './Components/Sidebar';

function App() {
  return (
    <Router>
      <div style={{ display: 'flex' }}>
        <Sidebar />
        <div style={{ flex: 1, padding: '20px' }}>
          <Routes>
            <Route path="/" element={<DashboardPage />} />
            <Route path="/profile" element={<Profile />} />
            <Route path="/data-sensor" element={<DataSensor />} />
            <Route path="/action-history" element={<ActionHistory />} />
          </Routes>
        </div>
      </div>
    </Router>
  );
}

export default App;
