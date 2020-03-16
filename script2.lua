print("[LUA] Hello! script2.lua here!")

selectedEntity = entityManager:SearchByIndex(m_index)

function Update()
    
    print("index: ", m_index);
    
    if input:GetKeyDown(32) then
        print("space")
    end
    
    print(selectedEntity:GetPosition())
end
