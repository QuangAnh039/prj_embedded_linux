import React from 'react';
import Sidebar from '../Components/Sidebar';
import SensorData from '../Components/SensorData';
import Chart from '../Components/Chart';
import CtrLed from '../Components/ControlLed';
import Box from '@mui/material/Box';

function DashboardPage() {
  return (
    <Box display="flex" flexDirection="row" height="100vh">
      {/* Sidebar */}
      <Box flex={1} style={{ width: '250px' }}>
        <Sidebar />
      </Box>

      {/* Main content area */}
      <Box flex={4} padding="20px">
        {/* Data and Chart */}
        <Box display="flex" flexDirection="column" height="100%">
          <Box marginBottom="20px">
            <SensorData />
          </Box>
          <Box display="flex" flexDirection="row" justifyContent="space-between">
            <Box flex={2} marginRight="20px">
              <Chart />
            </Box>
            <Box flex={1}>
              <CtrLed />
            </Box>
          </Box>
        </Box>
      </Box>
    </Box>
  );
}

export default DashboardPage;
