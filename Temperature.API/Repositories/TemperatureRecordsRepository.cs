using System.Data;
using Dapper;
using Temperature.API.Models;

namespace Temperature.API.Repositories;

public class TemperatureRecordsRepository : ITemperatureRecordsRepository
{
    private IDbConnection _connection;

    public TemperatureRecordsRepository(IDbConnection connection)
    {
        _connection = connection;
    }

    public async Task<List<TemperatureRecordDto>> GetInterval(GetTemperatureIntervalDto dto)
    {
        _connection.Open();

       var statement = 
        """
            select 
                cast(temperature as double precision) as temperature,
                record_timestamp as timestamp
            from temperature_records
            where 
                record_timestamp >= @IntervalStart
                and record_timestamp <= @IntervalEnd
        """;

        var results = await _connection.QueryAsync<TemperatureRecordDto>(statement, dto);
        return results.ToList();
    }


    public async Task Add(AddTemperatureRecordDto dto)
    {
        _connection.Open();

        var statement = 
        """
            insert into temperature_records (temperature)
            values (@Temperature)
        """;

        await _connection.ExecuteAsync(statement, dto);
    }
}